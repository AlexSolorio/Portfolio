//
//  BitVector.hpp
//  Homework7
//
//  Created by Alex Solorio on 3/5/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//

#ifndef BitVector_hpp
#define BitVector_hpp

#include <stdio.h>
#include <iostream>
#include "MyArray.hpp"

class BitVector {
public:
    BitVector(size_t numBitsToBeStored);
    ~BitVector();
    BitVector& operator=(const BitVector& source);
    BitVector(BitVector &);
    
    void resize(size_t numBitsToBeStored);
    void Fill();
    void Clear();
    bool getBit(size_t index) const;
    void setBit(size_t index, bool value);
    void toggleBit(size_t index);
    size_t getSize() const;
private:
    size_t getStorageSize(size_t numElements) const;
    void getElementAndOffset(size_t index, size_t &element, int &offset) const;
    MyArray<uint64_t>* storage;
    size_t numElements;
    size_t storageSize;
};

#endif /* BitVector_hpp */
