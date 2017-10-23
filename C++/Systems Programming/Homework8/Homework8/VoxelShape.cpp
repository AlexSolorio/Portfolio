//
//  VoxelShape.cpp
//  Homework7
//
//  Created by Alex Solorio on 3/5/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//
//  This class represents a 3D shape consisting of 3 dimensions of "voxels".
//  It uses a BitVector as its internal storage for the voxels and has functions allowing
//  us to add, subtract, or toggle spheres of voxels within the shape.
//

#include "VoxelShape.hpp"

VoxelShape::VoxelShape(int x, int y, int z):xDimension(x), yDimension(y), zDimension(z), data(x*y*z) {}

VoxelShape::~VoxelShape()
{
    xDimension = 0;
    yDimension = 0;
    zDimension = 0;
}

VoxelShape& VoxelShape::operator=(const VoxelShape& source)
{
    int size = source.xDimension*source.yDimension*source.zDimension;
    xDimension = source.xDimension;
    yDimension = source.yDimension;
    zDimension = source.zDimension;
    BitVector tmp{static_cast<size_t>(size)};
    tmp = source.data;
    data = tmp;
    return *this;
}

void VoxelShape::GetSize(int &x, int &y, int &z) const
{
    x = xDimension;
    y = yDimension;
    z = zDimension;
}

void VoxelShape::Fill()
{
    data.Fill();
}


void VoxelShape::Clear()
{
    data.Clear();
}


bool VoxelShape::getVoxel(int x, int y, int z) const
{
    return data.getBit(getIndex(x, y, z));
}


void VoxelShape::setVoxel(int x, int y, int z)
{
    data.setBit(getIndex(x, y, z), 1);
}


void VoxelShape::clearVoxel(int x, int y, int z)
{
    data.setBit(getIndex(x, y, z), 0);
}


void VoxelShape::toggleVoxel(int x, int y, int z)
{
    data.toggleBit(getIndex(x, y, z));
}


void VoxelShape::addSphere(float x, float y, float z, float r)
{
    int minX = std::max(0, static_cast<int>(x - r));
    int maxX = std::min(xDimension, static_cast<int>(x + r));
    int minY = std::max(0, static_cast<int>(y - r));
    int maxY = std::min(yDimension, static_cast<int>(y + r));
    int minZ = std::max(0, static_cast<int>(z - r));
    int maxZ = std::min(zDimension, static_cast<int>(z + r));
    for (int i = minX; i < maxX; i++)
    {
        for (int j = minY; j < maxY; j++)
        {
            for (int k = minZ; k < maxZ; k++)
            {
                if (((x-i)*(x-i) + (y - j)*(y - j) + (z - k)*(z - k)) < r*r)
                {
                    setVoxel(i, j, k);
                }
            }
        }
    }
}


void VoxelShape::subtractSphere(float x, float y, float z, float r)
{
    int minX = std::max(0, static_cast<int>(x - r));
    int maxX = std::min(xDimension, static_cast<int>(x + r));
    int minY = std::max(0, static_cast<int>(y - r));
    int maxY = std::min(yDimension, static_cast<int>(y + r));
    int minZ = std::max(0, static_cast<int>(z - r));
    int maxZ = std::min(zDimension, static_cast<int>(z + r));
    for (int i = minX; i < maxX; i++)
    {
        for (int j = minY; j < maxY; j++)
        {
            for (int k = minZ; k < maxZ; k++)
            {
                if (((x-i)*(x-i) + (y - j)*(y - j) + (z - k)*(z - k)) < r*r)
                {
                    clearVoxel(i, j, k);
                }
            }
        }
    }
}

void VoxelShape::toggleSphere(float x, float y, float z, float r)
{
    int minX = std::max(0, static_cast<int>(x - r));
    int maxX = std::min(xDimension, static_cast<int>(x + r));
    int minY = std::max(0, static_cast<int>(y - r));
    int maxY = std::min(yDimension, static_cast<int>(y + r));
    int minZ = std::max(0, static_cast<int>(z - r));
    int maxZ = std::min(zDimension, static_cast<int>(z + r));
    for (int i = minX; i < maxX; i++)
    {
        for (int j = minY; j < maxY; j++)
        {
            for (int k = minZ; k < maxZ; k++)
            {
                if (((x-i)*(x-i) + (y - j)*(y - j) + (z - k)*(z - k)) < r*r)
                {
                    toggleVoxel(i, j, k);
                }
            }
        }
    }
}

//helper function
size_t VoxelShape::getIndex(int x, int y, int z) const
{
    long index = (xDimension * yDimension * z) + (xDimension * y) + x;
    return index;
}

//inverse of getIndex()
void VoxelShape::getXYZ(int index, int& x, int& y, int& z)
{
    x = (index % (xDimension * yDimension)) % xDimension;
    y = (index % (xDimension * yDimension)) / xDimension;
    z = index / (xDimension * yDimension);
}
