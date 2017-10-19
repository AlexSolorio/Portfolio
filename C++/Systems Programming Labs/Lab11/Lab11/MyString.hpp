//
//  MyString.hpp
//  Lab11
//
//  Created by Alex Solorio on 2/15/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//
#include <stdio.h>
#include <iostream>
#ifndef MyString_hpp
#define MyString_hpp

class MyString {
public:
    MyString();
    MyString(const MyString &);
    MyString(const char *);
    ~MyString();
    MyString &operator=(const MyString &);
    size_t length() const;
    const char *cString() const;
private:
    char *string;
};



#endif /* MyString_hpp */
