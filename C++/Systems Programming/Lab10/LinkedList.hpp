//
//  LinkedList.hpp
//  Lab10
//
//  Created by Alex Solorio on 2/13/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//

#ifndef LinkedList_hpp
#define LinkedList_hpp

#include "Node.h"
#include <stdio.h>
#include <iostream>

class LinkedList{
private:
    Node* head;
public:
    LinkedList();
    void InsertAtHead(int v);
    void PrintList();
    void freeList();
};

#endif /* LinkedList_hpp */
