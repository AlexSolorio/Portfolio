//
//  VoxelShape.hpp
//  Homework7
//
//  Created by Alex Solorio on 3/5/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//

#ifndef VoxelShape_hpp
#define VoxelShape_hpp

#include <stdio.h>
#include "BitVector.hpp"

class VoxelShape {
public:
    VoxelShape(int x, int y, int z);
    void GetSize(int &x, int &y, int &z) const;
    ~VoxelShape();
    VoxelShape& operator=(const VoxelShape& source);
    void Fill();
    void Clear();
    
    bool getVoxel(int x, int y, int z) const;
    
    void setVoxel(int x, int y, int z);
    void clearVoxel(int x, int y, int z);
    void toggleVoxel(int x, int y, int z);
    
    void addSphere(float cx, float cy, float cz, float radius);
    void subtractSphere(float cx, float cy, float cz, float radius);
    void toggleSphere(float cx, float cy, float cz, float radius);
    
    void getXYZ(int index, int& x, int& y, int& z);
private:
    BitVector data;
    int xDimension, yDimension, zDimension;
    size_t getIndex(int x, int y, int z) const;
};

#endif /* VoxelShape_hpp */
