//
//  GroceryItem.hpp
//  Lab16
//
//  Created by Alex Solorio on 3/6/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//

#ifndef GroceryItem_hpp
#define GroceryItem_hpp

#include <stdio.h>

class GroceryItem {
public:
    virtual double GetWeight() = 0;
    virtual const char* GetDescription() = 0;
    virtual double GetPrice() = 0;
protected:
//    char* description;
//    double weight;
//    double price;
};

#endif /* GroceryItem_hpp */
