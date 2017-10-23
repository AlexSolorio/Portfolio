//
//  ChickenSoup.h
//  Lab16
//
//  Created by Alex Solorio on 3/6/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//

#ifndef ChickenSoup_h
#define ChickenSoup_h
#include "GroceryItem.hpp"

class ChickenSoup : public GroceryItem {
public:
    double GetWeight() {return 1.2;}
    const char* GetDescription() {return "Chicken Soup";}
    double GetPrice() {return 0.99;}
};


#endif /* ChickenSoup_h */
