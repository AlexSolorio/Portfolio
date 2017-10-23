//
//  CSG.cpp
//  Homework8
//
//  Created by Alex Solorio on 3/11/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//
//  This class allows us to add, subtract, mask, or toggle shapes to our .stl file.
//  It stores a list of shapes and operators, so that when it comes time to write the file,
//  it knows which voxels to turn on and off.
//

#include "CSG.hpp"

CSG::~CSG()
{
    for (ShapeListEntry e : shapeList)
    {
        delete e.thisShape;
        e.thisShape = nullptr;
        e.operation = 0;
    }
}

void convertToInputCoords(float& X, float& Y, float& Z, float ratio, float xOffset, float yOffset, float zOffset)
{
    X /= ratio;
    Y /= ratio;
    Z /= ratio;
    X += xOffset;
    Y += yOffset;
    Z += zOffset;
}

void CSG::RenderToFile(const char *filename, Point3D min, Point3D max, float widthInInches, int voxelsPerInch)
{
    float nx = widthInInches * voxelsPerInch;
    float inputChangeInX = max.x - min.x;
    float ratio = nx / inputChangeInX;
    
    float inputChangeInY = max.y - min.y;
    float inputChangeInZ = max.z - min.z;
    float ny = inputChangeInY * ratio;
    float nz = inputChangeInZ * ratio;
    
    float xOffset = min.x;
    float yOffset = min.y;
    float zOffset = min.z;
    
    VoxelShape model(nx,ny,nz);
    
    for (auto rit = shapeList.begin(); rit != shapeList.end(); rit++)
    {
        ShapeListEntry s = (*rit);
        Shape *thisShape = s.thisShape;
        
        
        switch (s.operation) {
                
            case '+':
                for (int i = 0; i < nx; i++)
                {
                    for (int j = 0; j < ny; j++)
                    {
                        for (int k = 0; k < nz; k++)
                        {
                            float inputX = i, inputY = j, inputZ = k;
                            convertToInputCoords(inputX, inputY, inputZ, ratio, xOffset, yOffset, zOffset);
                            if (thisShape->InsideShape(Point3D(inputX,inputY,inputZ)))
                            {
                                model.setVoxel(i, j, k);
                            }
                        }
                    }
                }
                break;
                
            case '-':
                for (int i = 0; i < nx; i++)
                {
                    for (int j = 0; j < ny; j++)
                    {
                        for (int k = 0; k < nz; k++)
                        {
                            float inputX = i, inputY = j, inputZ = k;
                            convertToInputCoords(inputX, inputY, inputZ, ratio, xOffset, yOffset, zOffset);
                            if (thisShape->InsideShape(Point3D(inputX,inputY,inputZ)))
                            {
                                model.clearVoxel(i, j, k);
                            }
                        }
                    }
                }
                break;
                
            case '&':
                for (int i = 0; i < nx; i++)
                {
                    for (int j = 0; j < ny; j++)
                    {
                        for (int k = 0; k < nz; k++)
                        {
                            float inputX = i, inputY = j, inputZ = k;
                            convertToInputCoords(inputX, inputY, inputZ, ratio, xOffset, yOffset, zOffset);
                            if (!(thisShape->InsideShape(Point3D(inputX,inputY,inputZ))))
                            {
                                model.clearVoxel(i, j, k);
                            }
                        }
                    }
                }
                break;
                
            case '^':
                for (int i = 0; i < nx; i++)
                {
                    for (int j = 0; j < ny; j++)
                    {
                        for (int k = 0; k < nz; k++)
                        {
                            float inputX = i, inputY = j, inputZ = k;
                            convertToInputCoords(inputX, inputY, inputZ, ratio, xOffset, yOffset, zOffset);
                            if (thisShape->InsideShape(Point3D(inputX,inputY,inputZ)))
                            {
                                model.toggleVoxel(i, j, k);
                            }
                        }
                    }
                }
                break;
                
            default:
                break;
        }

    }
    
    int numThreads = std::thread::hardware_concurrency();
    WriteSTL(model, filename, numThreads);
}

void CSG::Add(Shape * newShape)
{
    ShapeListEntry entry(newShape,'+');
    shapeList.push_back(entry);
}

void CSG::Subtract(Shape * newShape)
{
    ShapeListEntry entry(newShape,'-');
    shapeList.push_back(entry);
}

void CSG::Mask(Shape * newShape)
{
    ShapeListEntry entry(newShape,'&');
    shapeList.push_back(entry);
}

void CSG::Toggle(Shape * newShape)
{
    ShapeListEntry entry(newShape,'^');
    shapeList.push_back(entry);
}

CSG &CSG::operator+=(Shape * newShape)
{
    ShapeListEntry entry(newShape,'+');
    shapeList.push_back(entry);
    return *this;
}

CSG &CSG::operator-=(Shape * newShape)
{
    ShapeListEntry entry(newShape,'-');
    shapeList.push_back(entry);
    return *this;
}

CSG &CSG::operator&=(Shape * newShape)
{
    ShapeListEntry entry(newShape,'&');
    shapeList.push_back(entry);
    return *this;
}

CSG &CSG::operator^=(Shape * newShape)
{
    ShapeListEntry entry(newShape,'^');
    shapeList.push_back(entry);
    return *this;
}

