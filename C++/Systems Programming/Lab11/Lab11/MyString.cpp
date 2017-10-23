//
//  MyString.cpp
//  Lab11
//
//  Created by Alex Solorio on 2/15/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//

#include "MyString.hpp"

//default constructor
MyString::MyString():string(new char[1])
{
    string[0] = '\0';
}

//copy constructor
MyString::MyString(const MyString & source)
{
    long length = strlen(source.string);
    string = new char[length+1];
    for (long i = 0; i < length; i++)
    {
        string[i] = source.string[i];
    }
    string[length] = '\0';
}

//constructor taking in a char*
MyString::MyString(const char * source)
{
    long length = strlen(source);
    string = new char[length+1];
    for (long i = 0; i < length; i++)
    {
        string[i] = source[i];
    }
    string[length] = '\0';
}

//destructor
MyString::~MyString()
{
    delete[] string;
    string = nullptr;
}

MyString& MyString::operator=(const MyString & source)
{
    long length = source.length();
    char* tmp = new char[length+1];
    for (long i = 0; i < length; i++)
    {
        tmp[i] = source.string[i];
    }
    tmp[length] = '\0';
    
    delete[] string;
    string = tmp;
    return *this;
}

size_t MyString::length() const
{
    return strlen(string);
}

const char* MyString::cString() const
{
    return string;
}
