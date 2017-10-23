//
//  Celery.h
//  Lab16
//
//  Created by Alex Solorio on 3/6/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//

#ifndef Celery_h
#define Celery_h
#include "ProduceItem.h"

class Celery : public ProduceItem {
public:
    Celery(double newWeight) : ProduceItem(newWeight, 0.79) { }
    const char* GetDescription() {return "Celery";}
};

#endif /* Celery_h */
