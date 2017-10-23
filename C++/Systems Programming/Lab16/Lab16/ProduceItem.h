//
//  ProduceItem.h
//  Lab16
//
//  Created by Alex Solorio on 3/6/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//

#ifndef ProduceItem_h
#define ProduceItem_h

#include "GroceryItem.hpp"

class ProduceItem : public GroceryItem {
public:
    ProduceItem(double newWeight, double newCostPerPound)
    {
        costPerPound = newCostPerPound;
        weight = newWeight;
    }
    double GetWeight()
    {
        return weight;
    }
    double GetPrice()
    {
        return weight*costPerPound;
    }
protected:
    double costPerPound;
    double weight;
};

#endif /* ProduceItem_h */
