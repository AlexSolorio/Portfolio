//
//  main.cpp
//  Lab02
//
//  Created by Alex Solorio on 1/9/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//
//  Understanding and experimenting with C++ data storage of different data types and overflow errors
//

#include <iostream>

int main(int argc, const char * argv[]) {
    
    using std::cout;
    using std::cin;
    using std::endl;
    
    //demonstrating size of different types
    cout << "bool size: " << sizeof(bool) << endl;
    cout << "char size: " << sizeof(char) << endl;
    cout << "short size: " << sizeof(short) << endl;
    cout << "int size: " << sizeof(int) << endl;
    cout << "long size: " << sizeof(long) << endl;
    cout << "long long size: " << sizeof(long long int) << endl;
    cout << "float size: " << sizeof(float) << endl;
    cout << "double size: " << sizeof(double) << endl;
    cout << "long double size: " << sizeof(long double) << endl;
    cout << "int8_t size: " << sizeof(uint8_t) << endl;
    cout << "int16_t size: " << sizeof(uint16_t) << endl;
    cout << "int32_t size: " << sizeof(uint32_t) << endl;
    cout << "int64_t size: " << sizeof(uint64_t) << endl << endl;
    
    
    //printing decimal and hexidecimal
    cout << "Enter uint16_t number: ";
    uint16_t value;
    cin >> value;
    cout << "Decimal value is: " << std::dec << value << endl;
    cout << "Hex value is: " << std::hex << value << endl << endl;
    
    cout << "Enter uint32_t number: ";
    uint32_t value2;
    cin >> value2;
    cout << "Decimal value is: " << std::dec << value2 << endl;
    cout << "Hex value is: " << std::hex << value2 << endl << endl;
    
    cout << "Enter uint64_t number: ";
    uint32_t value3;
    cin >> value3;
    cout << "Decimal value is: " << std::dec << value3 << endl;
    cout << "Hex value is: " << std::hex << value3 << endl << endl;
    cout << std::dec;
    
    
    //demonstrating overflow
    uint8_t a = 255;
    cout << "Overflow for uint8_t: a = " << +a << endl;
    a++;
    cout << "after adding 1, a = " << +a << endl << endl;
    
    uint16_t b = 65535;
    cout << "Overflow for uint16_t: b = " << +b << endl;
    b++;
    cout << "after adding 1, b = " << +b << endl << endl;
    
    
    //unsigned maximum and minimum possible values
    uint32_t maxu32 = 0xffffffff;
    uint32_t minu32 = 0;
    cout << "Maximum for uint32_t is " << std::hex << maxu32 << " and minimum is " << minu32 << endl << endl;
    
    uint64_t maxu64 = 0xffffffffffffffff;
    uint64_t minu64 = 0x0;
    cout << "Maximum for uint64_t is " << std::hex << maxu64 << " and minimum is " << minu64 << endl << endl;
    
    
    //signed maximum and minimum possible values
    int32_t max32 = 0x7fffffff;
    int32_t min32 = 0x80000000;
    cout << "Maximum for int32_t is " << std::hex << max32 << " and minimum is " << min32 << endl << endl;
    
    int64_t max64 = 0x7fffffffffffffff;
    int64_t min64 = 0x8000000000000000;
    cout << "Maximum for int64_t is " << std::hex << max64 << " and minimum is " << min64 << endl << endl;
    
    
    //printing values of some uninitialized variables, integral and floating point.
    int32_t uninitialized;
    cout << "This int32_t is uninitialized: " << uninitialized <<endl;
    float alsoUninitialized;
    cout << "This float is uninitialized: " << alsoUninitialized <<endl;
    
    
    

}
