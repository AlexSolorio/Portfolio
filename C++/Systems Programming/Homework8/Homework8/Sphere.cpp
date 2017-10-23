//
//  Sphere.cpp
//  Homework8
//
//  Created by Alex Solorio on 3/13/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//

#include <stdio.h>
#include "Sphere.h"

Sphere::Sphere(Point3D center, float r)
:center(center), r(r)
{
}

bool Sphere::InsideShape(Point3D p0)
{
    if (distanceFrom(center, p0) <= r)
        return true;
    return false;
}

float Sphere::distanceFrom(Point3D p1, Point3D p2)
{
    return sqrt((p2.x-p1.x)*(p2.x-p1.x) + (p2.y-p1.y)*(p2.y-p1.y) + (p2.z-p1.z)*(p2.z-p1.z));
}
