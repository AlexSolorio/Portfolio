//
//  CSG.hpp
//  Homework8
//
//  Created by Alex Solorio on 3/11/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//

#ifndef CSG_hpp
#define CSG_hpp

#include <stdio.h>
#include "VoxelShape.hpp"
#include "Shape.h"
#include "Line.h"
#include "Sphere.h"
#include "Cube.h"
#include "Timer.h"
#include "surfaceExtraction.hpp"
#include <thread>
#include <vector>
//#include "SharedQueue.h"

class CSG {
public:
    CSG() {};
    ~CSG();
    CSG(const CSG&) = delete;
    CSG& operator=(const CSG&) = delete;
    
    void RenderToFile(const char *filename, Point3D min, Point3D max, float widthInInches, int voxelsPerInch = 256);
    void Add(Shape *);
    void Subtract(Shape *);
    void Mask(Shape *);
    void Toggle(Shape *);
    
    CSG &operator+=(Shape *);
    CSG &operator-=(Shape *);
    CSG &operator&=(Shape *);
    CSG &operator^=(Shape *);
    
    struct ShapeListEntry{
        ShapeListEntry(Shape *entry, char whichOperation)
        {
            thisShape = entry;
            operation = whichOperation;
        }
        Shape* thisShape;
        char operation;
    };
private:
    std::vector<ShapeListEntry> shapeList;
};



#endif /* CSG_hpp */
