//
//  main.cpp
//  Lab9
//
//  Created by Alex Solorio on 2/1/16.
//  Copyright (c) 2017 Alex Solorio. All rights reserved.
//
//  Created a simple LinkedList class capable of storing int values
//

#include <iostream>
#include "LinkedList.hpp"

int main(int argc, const char * argv[])
{
  LinkedList list;
  
  list.PrintList();
  
  for(int i=0; i < 10; i++)
  {
    list.InsertAtHead(i);
    list.PrintList();
  }
  
  list.PrintList();
  
  list.freeList();
  
  list.PrintList();
  
  return 0;
}
