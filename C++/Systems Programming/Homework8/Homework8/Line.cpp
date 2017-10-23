//
//  Line.cpp
//  Homework8
//
//  Created by Alex Solorio on 3/13/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//

#include <stdio.h>
#include "Line.h"

Line::Line(Point3D p1, Point3D p2, float r)
:p1(p1), p2(p2), r(r)
{
}

bool Line::InsideShape(Point3D p0)
{
    Point3D a = {p0.x-p1.x, p0.y-p1.y, p0.z-p1.z};
    Point3D b = {p0.x-p2.x, p0.y-p2.y, p0.z-p2.z};
    Point3D c = {p2.x-p1.x, p2.y-p1.y, p2.z-p1.z};
    float dist = magnitude ( cross(a, b) ) / magnitude(c);
    return dist <= r;;;
}

float Line::magnitude(Point3D p)
{
    return sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
}

Point3D Line::cross(Point3D p1, Point3D p2)
{
    Point3D result;
    result.x = p1.y*p2.z - p1.z*p2.y;
    result.y = p1.z*p2.x - p1.x*p2.z;
    result.z = p1.x*p2.y - p1.y*p2.x;
    return result;
}

float Line::distanceFrom(Point3D p1, Point3D p2)
{
    return sqrt((p2.x-p1.x)*(p2.x-p1.x) + (p2.y-p1.y)*(p2.y-p1.y) + (p2.z-p1.z)*(p2.z-p1.z));
}
