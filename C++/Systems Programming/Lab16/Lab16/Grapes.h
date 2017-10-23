//
//  Grapes.h
//  Lab16
//
//  Created by Alex Solorio on 3/6/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//

#ifndef Grapes_h
#define Grapes_h

#include "ProduceItem.h"

class Grapes : public ProduceItem {
public:
    Grapes(double newWeight) : ProduceItem(newWeight, 0.73) { }
    const char* GetDescription() {return "Grapes";}
};

#endif /* Grapes_h */
