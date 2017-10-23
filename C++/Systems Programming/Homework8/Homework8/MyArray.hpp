//
//  MyArray.hpp
//  Homework7
//
//  Created by Alex Solorio on 3/5/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//
//  A custom resizable array class designed to handle any element type (templated)
//

#ifndef MyArray_hpp
#define MyArray_hpp

#include <stdio.h>
#include <assert.h>

template <typename T>
class MyArray{
public:
    MyArray<T>(size_t initSize = 0);
    MyArray<T>(MyArray<T> &source);
    MyArray<T>& operator=(const MyArray<T> &source);
    ~MyArray<T>();
    
    T Get(size_t index) const;
    void Set(size_t index, T element);
    size_t Size();
    void Resize(size_t newSize);
    void PushBack(T element);
    T Back();
    T PopBack();
    
private:
    size_t arraySize;
    size_t numElements;
    T* array;
};



template <typename T>
MyArray<T>::MyArray(size_t initSize)
{
    array = new T[20];
    arraySize = 20;
    if (initSize <= 20)
        numElements = initSize;
    else{
        numElements = 20;
        Resize(initSize);
    }
}

template <typename T>
MyArray<T>::MyArray(MyArray<T> &source)
{
    size_t newArraySize = source.arraySize;
    size_t newNumElements = source.numElements;
    delete[] array;
    array = new T[newArraySize];
    for (int x = 0; x < newArraySize; x++)
        array[x] = source.array[x];
    arraySize = newArraySize;
    numElements = newNumElements;
}

template <typename T>
MyArray<T>& MyArray<T>::operator=(const MyArray<T> &source)
{
    if (&source == this)
        return *this;
    delete[] array;
    size_t newArraySize = source.arraySize;
    size_t newNumElements = source.numElements;
    array = new T[newArraySize];
    for (int x = 0; x < newArraySize; x++)
        array[x] = source.array[x];
    arraySize = newArraySize;
    numElements = newNumElements;
    return *this;
}

template <typename T>
MyArray<T>::~MyArray<T>()
{
    delete[] array;
    array = nullptr;
    arraySize = numElements = 0;
}

template <typename T>
T MyArray<T>::Get(size_t index) const
{
    assert(index < numElements && index >= 0);
    return array[index];
}

template <typename T>
void MyArray<T>::Set(size_t index, T element)
{
    assert(index < numElements && index >= 0);
    array[index] = element;
}

template <typename T>
size_t MyArray<T>::Size()
{
    return numElements;
}

template <typename T>
void MyArray<T>::Resize(size_t newSize)
{
    if (newSize > arraySize)
    {
        T* tmp = new T[newSize];
        for (size_t x = 0; x < numElements; x++)
            tmp[x] = array[x];
        numElements = newSize;
        arraySize = newSize;
        delete[] array;
        array = tmp;
    }
    else{
        numElements = newSize;
    }
}

template <typename T>
void MyArray<T>::PushBack(T element)
{
    size_t numberOfElements = numElements;
    if (numElements == arraySize)
        Resize(2*arraySize);
    numberOfElements++;
    numElements = numberOfElements;
    array[numElements-1] = element;
}

template <typename T>
T MyArray<T>::Back()
{
    return array[numElements-1];
}

template <typename T>
T MyArray<T>::PopBack()
{
    T lastItem = array[numElements-1];
    numElements--;
    return lastItem;
}
#endif /* MyArray_hpp */

