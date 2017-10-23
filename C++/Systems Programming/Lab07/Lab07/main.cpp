//
//  main.cpp
//  Lab07
//
//  Created by Alex Solorio on 1/30/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//
//  Experimenting with stack overflow and how Cstrings differ from strings in Java (array of chars vs object)
//

#include <iostream>

int overflow(int a);
char* reverseString(const char *input_string);

int main() {
    //int a = overflow(5);
    
    char c1[] = "zyxwvutsrqponmlkjihgfedcba";
    char *c2 = reverseString(c1);
    std::cout << c2 << std::endl;
    
    char c3[] = ".em htiw gnis uoy t'now emit txeN .sCBA ym wonk I woN";
    char *c4 = reverseString(c3);
    std::cout << c4 << std::endl;
    
    char c5[] = "";
    char *c6 = reverseString(c5);
    std::cout << c6 << std::endl;
    
    char c7[] = "\0";
    char *c8 = reverseString(c7);
    std::cout << c8 << std::endl;
    
    delete [] c2;
    delete [] c4;
    delete [] c6;
    delete [] c8;
}

//int overflow(int a)
//{
//    return overflow(a);
//}

char *reverseString(const char *input_string)
{
    char *newString = new char[strlen(input_string) + 1];
    int j = 0;
    for (int i = strlen(input_string) - 1; i >= 0; i--)
    {
        newString[j] = input_string[i];
        j++;
    }
    newString[j] = '\0';
    return newString;
}
