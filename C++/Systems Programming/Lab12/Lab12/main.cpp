//
//  main.cpp
//  Lab12
//
//  Created by Alex Solorio on 2/1/16.
//  Copyright (c) 2016 Alex Solorio. All rights reserved.
//
//  Updated my LinkedList class to handle multiple data types using templates
//

#include <iostream>
#include "LinkedList.hpp"
using TemplateLinkedList::LinkedList;

int main(int argc, const char * argv[])
{
    {
        LinkedList<std::string>* list = new LinkedList<std::string>();
        (*list).Print();
        for(int i = 0; i < 10; i++)
        {
            std::string tmp = "item " + std::to_string(i);
            (*list).InsertAtHead(tmp);
            (*list).Print();
        }
        for(int i = 0; i < 10; i++)
        {
            (*list).RemoveHead();
            (*list).Print();
        }
        
        
        delete list;
        (*list).Print();
    }
    
    {
        LinkedList<uint64_t>* list = new LinkedList<uint64_t>();
        (*list).Print();
        for(int i = 0; i < 10; i++)
        {
            (*list).InsertAtHead(i);
            (*list).Print();
        }
        for(int i = 0; i < 10; i++)
        {
            (*list).RemoveHead();
            (*list).Print();
        }
        
        
        delete list;
        (*list).Print();
    }
  
}
