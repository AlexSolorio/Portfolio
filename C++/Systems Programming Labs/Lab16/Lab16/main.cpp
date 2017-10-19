//
//  main.cpp
//  Lab 16
//
//  Practicing inheritance using grocery items and pricing as an example
//

#include <iostream>
#include "GroceryCart.hpp"

// These are sample items. Create your own
// unique items for your shopping cart!
#include "Celery.h"
#include "Lettuce.h"
#include "Potato.h"
#include "Grapes.h"
#include "ChickenSoup.h"

int main(int argc, const char * argv[])
{
    GroceryCart c;
    c.AddItemToCart(new Lettuce(1.2)); // Weight in lbs (Lettuce is $0.49/lb)
    c.AddItemToCart(new Celery(1.5));  // Weight in lbs (Celery is $0.79/lb)
    c.AddItemToCart(new Potato(3.1));
    c.AddItemToCart(new Grapes(4.1));
    c.AddItemToCart(new ChickenSoup);
    std::cout << c;
    return 0;
}

// Sample output
// 0.59	lettuce
// 1.19	celery
// 0.99	chicken soup
//-------
//$2.76
