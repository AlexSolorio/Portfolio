//
//  main.cpp
//  Homework8
//
//  Created by Alex Solorio on 3/11/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//
//  This is the final project created for my Systems Programming course at the University of Denver.
//  When it runs, the following three functions are called to create three .stl files (to be used for 3D printing/modeling)
//      1. A ball trapped within an outer ball
//      2. A Rubik's Cube design (does not turn, just shows outer slices)
//      3. My favorite (and took me the longest to design...): A 3D model of the string "C++" created only by adding and subtracting voxel spheres/cubes
//

#include <iostream>
#include "CSG.hpp"

void RenderBallInBall()
{
    CSG csg;
    Point3D middle = {0.0, 0.0, 0.0};
    Point3D lb = {-1, -1, -1};
    Point3D ub = {1, 1, 1};
    
    // Initial hollow sphere
    csg += new Sphere(middle, 1.0);
    csg ^= new Sphere(middle, 0.76);
    
    // 5 of 6 sides cut out (not bottom)
    csg -= new Sphere({ 1.0, 0.0,  0.0}, 0.4);
    csg -= new Sphere({-1.0, 0.0,  0.0}, 0.4);
    csg -= new Sphere({0.0,  1.0,  0.0}, 0.4);
    csg -= new Sphere({0.0, -1.0,  0.0}, 0.4);
    csg -= new Sphere({0.0,  0.0,  1.0}, 0.4);
    
    // Trim edges with cube
    csg &= new Cube(middle, 0.8);
    
    // Add floating ball in middle
    csg += new Sphere(middle, 0.44);
    
    Timer t;
    t.StartTimer();
    csg.RenderToFile("ballinball_064.stl", lb, ub, 2.0, 64);
    t.EndTimer();
    printf("%1.2fs total time elapsed\n", t.GetElapsedTime());
    
}

void RenderRubiksCube()
{
    CSG csg;
    Point3D middle = {0.5, 0.5, 0.5};
    Point3D bottomleftfront = {0,0,0};
    Point3D toprightback = {1, 1, 1};
    Point3D F1 = {1.0/3.0, 0.0, 0.0};
    Point3D F2 = {2.0/3.0, 0.0, 0.0};
    Point3D F3 = {1.0, 1.0/3.0, 0.0};
    Point3D F4 = {1.0, 2.0/3.0, 0.0};
    Point3D F5 = {2.0/3.0, 1.0, 0.0};
    Point3D F6 = {1.0/3.0, 1.0, 0.0};
    Point3D F7 = {0.0, 2.0/3.0, 0.0};
    Point3D F8 = {0.0, 1.0/3.0, 0.0};
    
    Point3D B1 = {1.0/3.0, 0.0, 1.0};
    Point3D B2 = {2.0/3.0, 0.0, 1.0};
    Point3D B3 = {1.0, 1.0/3.0, 1.0};
    Point3D B4 = {1.0, 2.0/3.0, 1.0};
    Point3D B5 = {2.0/3.0, 1.0, 1.0};
    Point3D B6 = {1.0/3.0, 1.0, 1.0};
    Point3D B7 = {0.0, 2.0/3.0, 1.0};
    Point3D B8 = {0.0, 1.0/3.0, 1.0};
    
    Point3D R1 = {1.0, 0.0, 1.0/3.0};
    Point3D R2 = {1.0, 0.0, 2.0/3.0};
    Point3D R3 = {1.0, 1.0, 2.0/3.0};
    Point3D R4 = {1.0, 1.0, 1.0/3.0};
    
    Point3D L1 = {0.0, 0.0, 1.0/3.0};
    Point3D L2 = {0.0, 0.0, 2.0/3.0};
    Point3D L3 = {0.0, 1.0, 2.0/3.0};
    Point3D L4 = {0.0, 1.0, 1.0/3.0};
    
    csg += new Cube(middle, 0.5);
    
    csg -= new Line(F1, F6, .02);
    csg -= new Line(F2, F5, .02);
    csg -= new Line(F4, F7, .02);
    csg -= new Line(F3, F8, .02);
    
    csg -= new Line(B1, B6, .02);
    csg -= new Line(B2, B5, .02);
    csg -= new Line(B4, B7, .02);
    csg -= new Line(B3, B8, .02);
    
    csg -= new Line(R1, R4, .02);
    csg -= new Line(R2, R3, .02);
    csg -= new Line(F4, B4, .02);
    csg -= new Line(F3, B3, .02);
    
    csg -= new Line(L1, L4, .02);
    csg -= new Line(L2, L3, .02);
    csg -= new Line(F7, B7, .02);
    csg -= new Line(F8, B8, .02);
    
    csg -= new Line(F1, B1, .02);
    csg -= new Line(F2, B2, .02);
    csg -= new Line(R1, L1, .02);
    csg -= new Line(R2, L2, .02);
    
    csg -= new Line(F6, B6, .02);
    csg -= new Line(F5, B5, .02);
    csg -= new Line(R4, L4, .02);
    csg -= new Line(R3, L3, .02);
    
    Timer t;
    t.StartTimer();
    csg.RenderToFile("rubiksCube.stl", bottomleftfront, toprightback, 2.0, 64);
    t.EndTimer();
    printf("%1.2fs total time elapsed\n", t.GetElapsedTime());
    
}

void RenderCPlusPlus()
{
    CSG csg;
    Point3D middle = {0.0, 0.0, 0.0};
    Point3D bottomleftfront = {-1, -1, -1};
    Point3D toprightback = {1, 1, 1};
    Point3D CircleMiddle = {-0.5, 0.0, 0.0};
    Point3D topCube = {-0.5, 1.0, 0.0};
    Point3D bottomCube = {-0.5, -1.0, 0.0};
    Point3D rightCube = {1.0, 0.0, 0.0};
    Point3D movingCubeCenter = {-0.3, 0.01, -0.3};
    
    csg += new Sphere(CircleMiddle, 0.5);
    csg -= new Sphere(CircleMiddle, 0.35);
    csg -= new Cube(topCube, 0.9);
    csg -= new Cube(bottomCube, 0.9);
    csg -= new Cube(rightCube, 1.3);
    for (float i = -0.3; i < 0.1; i += 0.1)
    {
        movingCubeCenter.z = i;
        csg += new Cube(movingCubeCenter, 0.05);
    }
    movingCubeCenter.z = -0.1;
    for (float i = -0.5; i < -0.1; i += 0.1)
    {
        movingCubeCenter.x = i;
        csg += new Cube(movingCubeCenter, 0.05);
    }
    movingCubeCenter.z = 0.0;
    for (float i = -0.1; i < 0.4; i += 0.1)
    {
        movingCubeCenter.x = i;
        csg += new Cube(movingCubeCenter, 0.05);
    }
    movingCubeCenter.x = 0.1;
    for (float i = -0.2; i < 0.3; i += 0.1)
    {
        movingCubeCenter.z = i;
        csg += new Cube(movingCubeCenter, 0.05);
    }
    
    
    Timer t;
    t.StartTimer();
    csg.RenderToFile("cpp.stl", bottomleftfront, toprightback, 2.0, 256);
    t.EndTimer();
    printf("%1.2fs total time elapsed\n", t.GetElapsedTime());
}

int main(int argc, const char * argv[])
{
    RenderCPlusPlus();
    RenderRubiksCube();
    RenderBallInBall();
}
