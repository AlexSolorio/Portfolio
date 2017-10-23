//
//  Sphere.h
//  Homework8
//
//  Created by Alex Solorio on 3/13/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//

#ifndef Sphere_h
#define Sphere_h

#include "Shape.h"
#include <math.h>

class Sphere : public Shape {
public:
    Sphere(Point3D center, float r);
    bool InsideShape(Point3D p);
private:
    float distanceFrom(Point3D p1, Point3D p2);
    Point3D center;
    float r;
};

#endif /* Sphere_h */
