//
//  Lettuce.h
//  Lab16
//
//  Created by Alex Solorio on 3/6/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//

#ifndef Lettuce_h
#define Lettuce_h

#include "ProduceItem.h"

class Lettuce : public ProduceItem {
public:
    Lettuce(double newWeight) : ProduceItem(newWeight, 0.49) { }
    const char* GetDescription() {return "Lettuce";}
};

#endif /* Lettuce_h */
