//
//  main.cpp
//  Lab11
//
//  Created by Alex Solorio on 2/15/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//
//  Created a simple string class with constructors, destructors, and operator =
//

#include <iostream>
#include "MyString.hpp"

int main(int argc, const char * argv[])
{
    MyString *s = new MyString();
    std::cout << "Empty: '" << s->cString() << "'\n";
    *s = "hello";
    std::cout << "After operator=: '" << s->cString() << "'\n";
    {
        MyString *t = new MyString(*s);
        std::cout << "After copy: '" << t->cString() << "'\n";
        s = t;
    }
    
    MyString *m = new MyString(" world");
    std::cout << "After constructor: '" << m->cString() << "'\n";
    m = s;
    std::cout << "After operator=: '" << m->cString() << "'\n";
    m = m;
    std::cout << "After operator=: '" << m->cString() << "'\n";
    
    return 0;
}
