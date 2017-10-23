//
//  Line.h
//  Homework8
//
//  Created by Alex Solorio on 3/11/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//

#ifndef Line_h
#define Line_h

#include "Shape.h"
#include <math.h>

class Line : public Shape {
public:
    Line(Point3D p1, Point3D p2, float r);
    bool InsideShape(Point3D p);
private:
    float magnitude(Point3D p);
    Point3D cross(Point3D p1, Point3D p2);
    float distanceFrom(Point3D p1, Point3D p2);
    Point3D p1, p2;
    float r;
};

#endif /* Line_h */
