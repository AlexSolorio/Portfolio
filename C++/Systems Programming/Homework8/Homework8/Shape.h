//
//  Shape.h
//  Homework8
//
//  Created by Alex Solorio on 3/11/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//
//  A base class for Line.cpp, Sphere.cpp, and Cube.cpp
//

#ifndef Shape_h
#define Shape_h

struct Point3D {
    Point3D(){}
    Point3D(double x, double y, double z) :x(x), y(y), z(z) {}
    Point3D(float x, float y, float z) :x(x), y(y), z(z) {}
    Point3D(int x, int y, int z) :x(x), y(y), z(z) {}
    float x, y, z;
};

class Shape {
public:
    virtual ~Shape() {}
    virtual bool InsideShape(Point3D p) = 0;
};

#endif /* Shape_h */
