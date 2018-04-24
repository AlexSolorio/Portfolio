
#ifndef SPN_H
#define SPN_H
#include <stdint.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include "Process.hpp"

/**
 * SPN Class - Stands for Shortest Process Next
 *              Used to run a simulation of the Shortest Process Next
 *              CPU scheduling algorithm for OS processes
 */
class SPN {
public:
    //constructor
    SPN(std::string name, float block_duration, float prediction_weight);
    
    //RULE OF FIVE
    SPN& operator=(SPN&&) = delete;         //move assignment
    SPN& operator=(const SPN&) = delete;    //copy assignment
    SPN(const SPN&) = delete;               //copy constructor
    SPN(SPN&&) = delete;                    //move constructor
    ~SPN();                                 //destructor
    
    //execute method
    void runSimulation();
    
    //Helper functions for runSimulation()
    void unblockProcesses(float simTime);
    float getAverageTurnaroundTime();
private:
    std::fstream myFile;
    
    std::priority_queue<Process> readyProcesses;
    std::vector<Process> blockedProcesses;
    
    std::vector<float> turnAroundTimes;
    
    float block_duration;
    float prediction_weight;
};

#endif /* SPN_H */
