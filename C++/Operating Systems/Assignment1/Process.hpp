//
//  Process.hpp
//  OS Assignment 1
//
//  Created by Alex Solorio on 4/16/18.
//  Copyright © 2018 Alex Solorio. All rights reserved.
//

#ifndef Process_hpp
#define Process_hpp

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

/**
 * Process Struct - Represents an individual process in the simulation
 *                  of SPN process scheduling. Contains relevant variables
 *                  and functions for SPN calculations
 */
struct Process {
    
    //name of process
    std::string processName;
    
    //list of CPU time slots required for this specific process
    std::vector<float> timeSlots;
    
    //arrival time of process to CPU
    float arrival;
    
    //value used in calculation to predict the next runtime of this process
    float predictionValue;
    
    //time that this process was last blocked
    float timeBlocked;
    
    bool operator< (const Process& other) const;
};

#endif /* Process_hpp */