//
//  BitVector.cpp
//  Homework7
//
//  Created by Alex Solorio on 3/5/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//
//  A resizable storage class for storing individual bits of data.
//  This class is useful for our .stl file creation because it allows us to store individual "voxels" as either
//  "on" or "off", requiring only one bit of storage per voxel while we create the voxel models.
//

#include "BitVector.hpp"
#include "MyArray.hpp"

BitVector::BitVector(size_t numBitsToBeStored)
{
    numElements = numBitsToBeStored;
    storageSize = getStorageSize(numBitsToBeStored);
    storage = new MyArray<uint64_t>(storageSize);
    Clear();
}

BitVector::BitVector(BitVector &source)
{
    size_t size = source.storageSize;
    storage = new MyArray<uint64_t>(size);
    for (int x = 0; x < size; x++)
        (*storage).Set(x, (*source.storage).Get(x));
    storageSize = size;
    numElements = source.numElements;
}

BitVector::~BitVector()
{
    numElements = storageSize = 0;
    delete storage;
}

BitVector& BitVector::operator=(const BitVector& source)
{
    if (&source == this)
        return *this;
    delete storage;
    size_t size = source.storageSize;
    storage = new MyArray<uint64_t>(size);
    for (int x = 0; x < size; x++)
        storage->Set(x, (*source.storage).Get(x));
    storageSize = size;
    numElements = source.numElements;
    return *this;
}

//changes the number of bits that can be stored
void BitVector::resize(size_t newSize)
{
    storage->Resize(newSize);
    numElements = newSize;
    storageSize = getStorageSize(newSize);
}

//set all bits in the BitVector to 1
void BitVector::Fill()
{
    for (int x = 0; x < storageSize; x++)
        storage->Set(x, -1ULL);
}

//set all bits in the BitVector to 0
void BitVector::Clear()
{
    for (int x = 0; x < storageSize; x++)
        storage->Set(x, 0);
}

//return the value (true =  1, false = 0) at the given index
bool BitVector::getBit(size_t index) const
{
    size_t element;
    int offset;
    getElementAndOffset(index, element, offset);
    return (storage->Get(element)>>offset) & 0x1;
}

//set the value (true =  1, false = 0) at the given index
void BitVector::setBit(size_t index, bool value)
{
    uint64_t one = 1;
    size_t element;
    int offset;
    getElementAndOffset(index, element, offset);
    if (value)
    {
        uint64_t tmp = storage->Get(element) | (one<<offset);
        storage->Set(element, tmp);
    }
    else{
        uint64_t tmp = storage->Get(element) & (~(one<<offset));
        storage->Set(element, tmp);
    }
}

void BitVector::toggleBit(size_t index)
{
    uint64_t one = 1;
    size_t element;
    int offset;
    getElementAndOffset(index, element, offset);
    uint64_t tmp = storage->Get(element) ^ (one<<offset);
    storage->Set(element, tmp);
}

//returns the number of bits that can be stored
size_t BitVector::getSize() const
{
    return numElements;
}

size_t BitVector::getStorageSize(size_t numElements) const
{
    size_t StorageSize = numElements>>6;
    if (numElements & 0x3F)
        return StorageSize+1;
    return StorageSize;
}

void BitVector::getElementAndOffset(size_t index, size_t &element, int &offset) const
{
    element = index>>6;
    offset = index&0x3F;
}
