//
//  main.cpp
//  Lab04
//
//  Created by Alex Solorio on 1/18/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//
//  Understanding the difference between passing data into functions by value, by reference, and using pointers
//

#include <iostream>
using std::cout;
using std::endl;

void swap_value(int a, int b)
{
    int temp = a;
    a = b;
    b = temp;
}

void swap_reference(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

void swap_pointer(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

struct pair {
    int a = 1;
    int b = 2;
};

void swap_value(pair p)
{
    int temp = p.a;
    p.a = p.b;
    p.b = temp;
}

void swap_reference(pair &p)
{
    int temp = p.a;
    p.a = p.b;
    p.b = temp;
}

void swap_pointer(pair *p)
{
    int temp = (*p).a;
    (*p).a = (*p).b;
    (*p).b = temp;
}

struct largeStruct {
    int a[100000];
};

void iterateStruct1 (largeStruct q)
{
    for(int i = 1; i < 100000; ++i)
    {
        q.a[i] = i * (i-1);
    }
}

void iterateStruct2 (largeStruct &q)
{
    for(int i = 1; i < 100000; ++i)
    {
        q.a[i] = i * (i-1);
    }
}

int main(int argc, const char * argv[])
{
    
    
    int8_t blah = ~0x51 + 1;
    cout << +blah;
    
    int a = 1;
    int b = 2;
    int &A = a;
    int &B = b;
    int *a_p = &a;
    int *b_p = &b;
    
    //does not swap properly
    cout << a << "," << b << endl;
    swap_value(a,b);
    cout << a << "," << b << endl << endl;
    
    //does swap properly
    cout << a << "," << b << endl;
    swap_reference(A,B);
    cout << a << "," << b << endl << endl;
    
    //does swap properly
    cout << a << "," << b << endl;
    swap_pointer(&a,&b);
    cout << a << "," << b << endl << endl;
    
    
    pair p;
    pair &P = p;
    pair *p_pointer = &p;
    
    //does not swap properly
    cout << p.a << "," << p.b << endl;
    swap_value(p);
    cout << p.a << "," << p.b << endl << endl;
    
    //does swap properly
    cout << p.a << "," << p.b << endl;
    swap_reference(P);
    cout << p.a << "," << p.b << endl << endl;
    
    //does swap properly
    cout << p.a << "," << p.b << endl;
    swap_pointer(p_pointer);
    cout << p.a << "," << p.b << endl << endl;
    
    
    largeStruct q;
    largeStruct &Q = q;
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    
    for(int i = 1; i < 10000; i++)
    {
        iterateStruct1(q);
    }
    
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "Elapsed time for first algorithm: " << elapsed_seconds.count() << "s\n";
    
    start = std::chrono::system_clock::now();
    
    for(int i = 1; i < 10000; i++)
    {
        iterateStruct2(Q);
    }
    
    end = std::chrono::system_clock::now();
    elapsed_seconds = end-start;
    std::cout << "Elapsed time for second algorithm: " << elapsed_seconds.count() << "s\n";
    
    return 0;
}
