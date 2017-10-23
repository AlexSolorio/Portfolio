//
//  Cube.cpp
//  Homework8
//
//  Created by Alex Solorio on 3/13/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//

#include <stdio.h>
#include "Cube.h"

Cube::Cube(Point3D center, float r)
:center(center), r(r)
{
    topLeft.x = center.x + r;
    topLeft.y = center.y + r;
    topLeft.z = center.z + r;
    bottomRight.x = center.x - r;
    bottomRight.y = center.y - r;
    bottomRight.z = center.z - r;
}

bool Cube::InsideShape(Point3D p)
{
    if (topLeft.x >= p.x && p.x >= bottomRight.x && topLeft.y >= p.y && p.y >= bottomRight.y && topLeft.z >= p.z && p.z >= bottomRight.z)
    {
        return true;
    }
    return false;
}
