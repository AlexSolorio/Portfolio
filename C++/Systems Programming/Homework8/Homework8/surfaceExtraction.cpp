//
//  surfaceExtraction.cpp
//  Homework7
//
//  Created by Ben Jones on 1/6/17.
//  Copyright © 2017 Ben Jones. All rights reserved.
//
//  Ben Jones (one of my professors) wrote the FillPlane() and ExtractFace() classes below, and the rest were written by myself.
//  This class has the code that actually writes the .stl file.
//  It writes the binary file in parallel using the Threadworker function
//
#define _crt_secure_no_warnings
#include "surfaceExtraction.hpp"
#include "VoxelShape.hpp"
#include "SharedQueue.h"
#include <mutex>
#include <cstdio>
#include <cstdint>
#include <thread>

struct Range {
    int x, y;
};

enum FaceType {
    NX,
    NY,
    NZ,
    PX,
    PY,
    PZ
};

struct Triangle{
    float normal[3];
    float v1[3];
    float v2[3];
    float v3[3];
};

inline void FillPlane(int a1, int a2, int b1, int b2, int c, int cInd, Triangle& t1, Triangle& t2)
{
    t1.v1[cInd] = c;
    t2.v1[cInd] = c;
    t1.v2[cInd] = c;
    t2.v2[cInd] = c;
    t1.v3[cInd] = c;
    t2.v3[cInd] = c;
    int aInd = (cInd +1) % 3;
    int bInd = (cInd +2) % 3;
    
    t1.v1[aInd] = a1;
    t1.v2[aInd] = a2;
    t1.v3[aInd] = a2;
    
    t2.v1[aInd] = a1;
    t2.v2[aInd] = a2;
    t2.v3[aInd] = a1;
    
    t1.v1[bInd] = b1;
    t1.v2[bInd] = b1;
    t1.v3[bInd] = b2;
    
    t2.v1[bInd] = b1;
    t2.v2[bInd] = b2;
    t2.v3[bInd] = b2;
    
}

void ExtractFace(int x, int y, int z, FaceType face, Triangle& t1, Triangle& t2)
{
    for (int i= 0; i < 3; i++)
    {
        t1.normal[i] = 0;
        t2.normal[i] = 0;
    }
    switch(face){
        case NX:
            t1.normal[0] = -1;
            t2.normal[0] = -1;
            FillPlane(y + 1, y, z, z+1, x, 0, t1, t2);
            break;
        case NY:
            t1.normal[1] = -1;
            t2.normal[1] = -1;
            FillPlane(z + 1, z, x, x+1, y, 1, t1, t2);
            break;
        case NZ:
            t1.normal[2] = -1;
            t2.normal[2] = -1;
            FillPlane(x + 1, x, y, y + 1, z, 2, t1, t2);
            break;
        case PX:
            t1.normal[0] = 1;
            t2.normal[0] = 1;
            FillPlane(y, y + 1, z, z +1, x + 1, 0, t1, t2);
            break;
        case PY:
            t1.normal[1] = 1;
            t2.normal[1] = 1;
            FillPlane(z, z + 1, x, x + 1, y + 1, 1, t1, t2);
            break;
        case PZ:
            t1.normal[2] = 1;
            t2.normal[2] = 1;
            FillPlane(x, x +1, y, y + 1, z + 1, 2, t1, t2);
            break;
    }
}

void WriteTriangle(FILE* f, const Triangle& t){
    fwrite(t.normal, sizeof(float), 3, f);
    fwrite(t.v1, sizeof(float), 3, f);
    fwrite(t.v2, sizeof(float), 3, f);
    fwrite(t.v3, sizeof(float), 3, f);
    uint16_t zero = 0;
    fwrite(&zero, sizeof(zero), 1, f);
}

void checkSurroundingVoxels(int x, int y, MyArray<Triangle>& array, const VoxelShape &model);

void ThreadWorker(MyArray<Triangle>& array, SharedQueue<Range> *workQueue, std::mutex& lock, const VoxelShape &model)
{
    Range r;
    MyArray<Triangle> thisThreadArray;
    while (true)
    {
        // read work from queue
        while (workQueue->Remove(r) == false)
        { std::this_thread::yield(); }
        
        // if x == y == -1, then this is a quit message; we are done
        if (r.x == -1 && r.y == -1)
        {
            break;
        }
        
        checkSurroundingVoxels(r.x, r.y, thisThreadArray, model);
    }
    lock.lock();
    size_t size = thisThreadArray.Size();
    for (int x = 0; x < size; x++)
        array.PushBack(thisThreadArray.PopBack());
    lock.unlock();
}

void checkSurroundingVoxels(int x, int y, MyArray<Triangle>& array, const VoxelShape &model)
{
    Triangle t1, t2;
    int nx, ny, nz;
    model.GetSize(nx, ny, nz);
    for (int z = 0; z < nz; z++)
    {
        if (model.getVoxel(x, y, z))
        {
            if ( ((x -1) < 0) || !model.getVoxel(x - 1, y, z))
            {
                ExtractFace(x, y, z, NX, t1, t2);
                array.PushBack(t1);
                array.PushBack(t2);
            }
            if ( ((x +1) >= nx) || !model.getVoxel(x + 1, y, z))
            {
                ExtractFace(x, y, z, PX, t1, t2);
                array.PushBack(t1);
                array.PushBack(t2);
            }
            if ( ((y -1) < 0) || !model.getVoxel(x, y -1, z))
            {
                ExtractFace(x, y, z, NY, t1, t2);
                array.PushBack(t1);
                array.PushBack(t2);
            }
            if ( ((y + 1) >= ny) || !model.getVoxel(x, y+1, z))
            {
                ExtractFace(x, y, z, PY, t1, t2);
                array.PushBack(t1);
                array.PushBack(t2);
            }
            if ( (z - 1 < 0) || !model.getVoxel(x, y, z-1) )
            {
                
                ExtractFace(x, y, z, NZ, t1, t2);
                array.PushBack(t1);
                array.PushBack(t2);
                
            }
            if ( (z + 1 >= nz) || !model.getVoxel(x, y, z +1) )
            {
                ExtractFace(x, y, z, PZ, t1, t2);
                array.PushBack(t1);
                array.PushBack(t2);
            }
        }
    }
}

void WriteSTL(const VoxelShape &model, const char* filename, int numThreads){
    
    FILE* f = fopen(filename, "wb");
    uint8_t header[80] = {0};
    fwrite(header, sizeof(uint8_t), 80, f);
    uint32_t zero = 0;
    fwrite(&zero, sizeof(uint32_t), 1, f);
    int nx, ny, nz;
    model.GetSize(nx, ny, nz);
    MyArray<Triangle> array;
    
    SharedQueue<Range> *workQueue = new SharedQueue<Range>;
    std::mutex lock;
    std::thread **threads;
    threads = new std::thread*[numThreads];
    
    for (int x = 0; x < numThreads; x++)
    {
        threads[x] = new std::thread(ThreadWorker, std::ref(array), workQueue, std::ref(lock), std::ref(model));
    }
    
    for (int x = 0; x < nx; x++)
    {
        for (int y = 0; y < ny; y++)
        {
            workQueue->Add({x,y});
        }
    }
    
    for (int x = 0; x < numThreads; x++)
    {
        workQueue->Add({-1,-1});
    }
    
    for (int x = 0; x < numThreads; x++)
    {
        threads[x]->join();
        delete threads[x];
    }
    delete [] threads;
    delete workQueue;
    
    //write all triangles in the MyArray
    size_t size = array.Size();
    for (size_t x = 0; x < size; x++)
    {
        WriteTriangle(f, array.PopBack());
    }
    fseek(f, 80, SEEK_SET);
    fwrite(&size, sizeof(uint32_t), 1, f);
    fclose(f);
}
