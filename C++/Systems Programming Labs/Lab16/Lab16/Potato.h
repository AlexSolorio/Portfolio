//
//  Potato.h
//  Lab16
//
//  Created by Alex Solorio on 3/6/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//

#ifndef Potato_h
#define Potato_h

#include "ProduceItem.h"

class Potato : public ProduceItem {
public:
    Potato(double newWeight) : ProduceItem(newWeight, 1.23) { }
    const char* GetDescription() {return "Potato";}
};

#endif /* Potato_h */
