#include <iostream>

#include "PackedArray.hpp"


// Create the array; initialize memory
PackedArray CreateArray(int aSize)
{
    PackedArray a;
    a.numRows = aSize;
    a.rows = new uint64_t[a.numRows];
    return a;
}

// Frees memory for the array
void FreeArray(PackedArray &array)
{
    delete[] array.rows;
    array.rows = nullptr;
}

// Sets all entries in the array to value parameter
void FillArray(PackedArray &array, uint64_t value)
{
    for(int row = 0; row < array.numRows; row++)
    {
        for (int col = 0; col < 32; col++)
        {
            SetEntry(array, row, col, value);
        }
    }
}

// Clears all entries to 0 in the array
void ClearArray(PackedArray &array)
{
    for(int row = 0; row < array.numRows; row++)
    {
        for (int col = 0; col < 32; col++)
        {
            SetEntry(array, row, col, 0);
        }
    }
}

// Get the value of the entry at the specified row and col.
// If row or col are outside of the range for this PackedArray,
// returns 0.
int GetEntry(const PackedArray &array, int row, int col)
{
    uint64_t returnValue = array.rows[row];
    returnValue >>= (col*2);
    returnValue &= 3ULL;
    return (int)returnValue;
}

// Sets entry in the location row, col to the specified value.
// If row or col are outside of the range for this PackedArray,
// function has no effect.
void SetEntry(PackedArray &array, int row, int col, uint64_t value)
{
    if (row >= array.numRows || row < 0 || col > 31 || col < 0)
        return;
    
    //clear two entries
    const uint64_t three = 3;
    array.rows[row] &= ~(three << col*2);
    
    //set entries
    array.rows[row] |= (value << col*2);
}

// Sum all the entries in the array
int SumEntries(const PackedArray &array)
{
    int sum = 0;
    for(int row = 0; row < array.numRows; row++)
    {
        for (int col = 0; col < 32; col++)
        {
            sum += GetEntry(array, row, col);
        }
    }
    return sum;
}
