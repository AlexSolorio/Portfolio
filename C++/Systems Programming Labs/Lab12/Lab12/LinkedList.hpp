//
//  LinkedList.hpp
//  Lab12
//
//  Created by Alex Solorio on 2/20/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//

#ifndef LinkedList_hpp
#define LinkedList_hpp

#include <stdio.h>
#include <iostream>

namespace TemplateLinkedList{
    
    template <typename T> class LinkedList{
    private:
        struct Node{
            T item;
            Node* nextNode;
            Node(T v = 0, Node* n = NULL):item(v), nextNode(n){}
            
            void SetValue(T v) {item = v;}
            
            void SetNext(Node* n){nextNode = n;}
            
            T GetValue() const {return item;}
            
            Node* GetNext() const {return nextNode;}
            
            void PrintNode(){std::cout <<"("<<item<<")";}
        };
        
        Node *head;
    public:
        LinkedList();
        ~LinkedList();
        bool isEmpty();
        void InsertAtHead(T item);
        T PeekHead();
        T RemoveHead();
        void Print();
    };
    
    template <typename T>
    LinkedList<T>::LinkedList(): head(nullptr){}
    
    template <typename T>
    LinkedList<T>::~LinkedList(){
        Node* loop_pointer = head;
        while (loop_pointer != nullptr){
            Node* temp_pointer = loop_pointer;
            loop_pointer = loop_pointer->GetNext();
            temp_pointer->SetNext(nullptr);
            delete temp_pointer;
        }
        head = nullptr;
    }
    
    template <typename T>
    bool LinkedList<T>::isEmpty()
    {
        if (head == nullptr)
            return true;
        return false;
    }
    
    template <typename T>
    void LinkedList<T>::InsertAtHead(T item){
        Node *n = new Node(item, this->head);
        this->head = n;
    }
    
    template <typename T>
    T LinkedList<T>::PeekHead(){
        return *head;
    }
    
    template <typename T>
    T LinkedList<T>::RemoveHead(){
        T returnValue = head->GetValue();
        if (isEmpty())
            return returnValue;
        
        Node* tmp = head;
        head = (*head).GetNext();
        delete tmp;
        return returnValue;
    }
    
    template <typename T>
    void LinkedList<T>::Print(){
        Node* temp_pointer = head;
        while (temp_pointer != nullptr){
            temp_pointer->PrintNode();
            temp_pointer = temp_pointer->GetNext();
        }
        std::cout << "\n";
    }
    
}

#endif /* LinkedList_hpp */
