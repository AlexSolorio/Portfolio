//
//  Process.cpp
//  OS Assignment 1
//
//  Created by Alex Solorio on 4/16/18.
//  Copyright © 2018 Alex Solorio. All rights reserved.
//

#include "Process.hpp"

/**
 * Defining comparison operator in order for std::priority_queue to work correctly
 *
 * @param other         the Process that we are comparing to
 *
 */
bool Process::operator< (const Process& other) const {
    
    //if two prediction values are the same, compare arrival times instead
    if (Process::predictionValue == other.predictionValue){
        return Process::arrival > other.arrival;
    }
    else{
        return Process::predictionValue > other.predictionValue;
    }
}