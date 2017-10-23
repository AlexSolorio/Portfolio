//  Created a PackedArray class that uses bitwise manipulations to store grid data (rows and columns) into a single array


#include <iostream>
#include <stdio.h>
#include <cstdint>

#include "PackedArray.hpp"
#include "TestLab6.hpp"


int main(int argc, const char * argv[])
{
    // Run tests with various size PackedArray
    std::cout << "Tests running.\n";
    TestLab6(4);
    
    TestLab6(6);
    
    TestLab6(2);
    
    TestLab6(64);
    
    TestLab6(32);
    
    TestLab6(33);
    
    TestLab6(1001);
    
    std::cout << "Testing complete.\n";
}
