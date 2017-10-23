//
//  LinkedList.cpp
//  Lab10
//
//  Created by Alex Solorio on 2/13/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//

#include "LinkedList.hpp"

LinkedList::LinkedList(): head(nullptr){}

void LinkedList::InsertAtHead(int v){
    Node *n = new Node(v, this->head);
    this->head = n;
}

void LinkedList::PrintList(){
    Node* temp_pointer = head;
    while (temp_pointer != nullptr){
        temp_pointer->PrintNode();
        temp_pointer = temp_pointer->GetNext();
    }
    std::cout << "\n";
}

void LinkedList::freeList(){
    Node* loop_pointer = head;
    while (loop_pointer != nullptr){
        Node* temp_pointer = loop_pointer;
        loop_pointer = loop_pointer->GetNext();
        temp_pointer->SetNext(nullptr);
        delete temp_pointer;
    }
    head = nullptr;
}
