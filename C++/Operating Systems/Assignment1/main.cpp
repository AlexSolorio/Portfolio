//
//  main.cpp
//  OS Assignment 1
//
//  Created by Alex Solorio on 4/11/18.
//  Copyright © 2018 Alex Solorio. All rights reserved.
//

#include <iostream>
#include "Process.hpp"
#include "SPN.hpp"
#include <string>

int main(int argc, const char * argv[]) {
    
    if (argc < 4){
        std::cerr << "Please specify the input filename, block_duration, and prediction_weight as the 3 command-line args\n";
        return 1;
    }
    
    //get command line arguments
    std::string fileName = argv[1];
    float block_duration = std::stof(argv[2]);
    float prediction_weight = std::stof(argv[3]);
    
    //print first line of output
    std::cout << fileName << " " << block_duration << " " << prediction_weight << std::endl;
    
    SPN spn(fileName, block_duration, prediction_weight);
    
    //run the Shortest Process Next algorithm
    spn.runSimulation();
    
    return 0;
}
