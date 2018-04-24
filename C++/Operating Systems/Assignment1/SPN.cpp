
#include "SPN.hpp"
#include <stdint.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cstdlib>

using std::istringstream;

/**
 * SPN Constructor - Creates an SPN class instance
 *
 * @param filename             the file from which to read in Process information
 * @param block_duration       the time that each Process will block for after running
 * @param prediction_weight    the prediction value used in the computation of predicted Process runtimes
 *
 */
SPN::SPN(std::string filename, float block_duration, float prediction_weight) {
    //open file
    SPN::myFile.open(filename, std::fstream::in | std::fstream::out);
    SPN::block_duration = block_duration;
    SPN::prediction_weight = prediction_weight;
}

/**
 * SPN Destructor - Deletes an SPN class instance
 *
 */
SPN::~SPN() {
    //close file
    SPN::myFile.close();
}

/**
 * runSimulation -  Runs the simulation of the Shortest Process Next CPU scheduling algorithm
 *                  using the block duration, prediction weight, and input file specified in the constructor
 */
void SPN::runSimulation(){
    Process a;
    
    try{
        //check if file successfully open
        if(myFile.fail()){
            std::string er = "Unable to Open\n";
            throw std::runtime_error(er);
        }
        
        //save individual line of text
        std::string myLine;
        
        //time to be used later in simulation
        float simTime = 0.0;
        
        //read in all processes from input file into readyProcesses queue
        while(getline(myFile,myLine)){
            istringstream getLine(myLine);
            Process tmpProcess;
            
            //get process name
            std::string pName;
            getLine >> pName;
            tmpProcess.processName = pName;  //set name
            
            //get arrival time
            float aTime;
            getLine >> aTime;
            tmpProcess.arrival = aTime; //set arrival time
            
            //get time slots
            float timeSlot;
            while(getLine >> timeSlot)
            {
                tmpProcess.timeSlots.push_back(timeSlot);
            }
            
            //set process predictionValue to 0
            tmpProcess.predictionValue = 0.0;
            
            //set timeBlocked
            tmpProcess.timeBlocked = 0.0;
            
            //add tmpProcess to the processes array
            SPN::readyProcesses.push(tmpProcess);
        }
        
        //Run processes until all are finished, printing key information to standard output
        while (true)
        {
            //at least one process is ready to be run
            if (SPN::readyProcesses.size() > 0){
                float processStartTime = simTime;
                
                //use Priority Queue to find correct process to run
                Process tmpProcess = SPN::readyProcesses.top();
                SPN::readyProcesses.pop();
                
                //if we found a process with no more runtimes, we remove it from the ready queue
                if (tmpProcess.timeSlots.size() < 1){
                    std::cerr << "ERROR: Process in ready queue without any remaining timeslots.\n";
                    exit(1);
                }
                
                //store and delete the runtime of this process
                float currentProcessRunTime = tmpProcess.timeSlots.at(0);
                tmpProcess.timeSlots.erase(tmpProcess.timeSlots.begin());
                
                //print stuff for this process
                std::cout   << processStartTime << "\t"
                            << tmpProcess.processName << "\t"
                            << currentProcessRunTime << "\t";
                
                
                //update prediction value for this process
                if (tmpProcess.predictionValue == 0.0){
                    tmpProcess.predictionValue = currentProcessRunTime;
                } else {
                    tmpProcess.predictionValue =
                                (SPN::prediction_weight * tmpProcess.predictionValue) +
                                ((1.0 - SPN::prediction_weight) * currentProcessRunTime);
                }
                
                //update simTime
                simTime += currentProcessRunTime;
                
                //if this process is out of timeSlots (will not run anymore), store its turnaround time
                //otherwise, move this process into the blocked vector and update it's "timeBlocked" variable
                if (tmpProcess.timeSlots.size() < 1){
                    SPN::turnAroundTimes.push_back(simTime - tmpProcess.arrival);
                    std::cout << "T\t\n";
                } else {
                    std::cout << "B\t" << tmpProcess.predictionValue << "\n";
                    tmpProcess.timeBlocked = simTime;
                    SPN::blockedProcesses.push_back(tmpProcess);
                }
                
                //go through blocked vector and unblock any processes that should be unblocked
                unblockProcesses(simTime);

            }
            
            else { //There are no ready processes, idle interval
                
                //are there any blocked processes? And if so, do those blocked processes still need to run?
                if (SPN::blockedProcesses.size() > 0 && (SPN::blockedProcesses.at(0).timeSlots.size() > 0)){
                    
                    float lowestTimeBlocked = SPN::blockedProcesses.at(0).timeBlocked;
                    
                    //To figure out how long the idle interval must be, we loop through the blocked
                    //processes and find the one with the lowest "timeBlocked" variable
                    for (int i = 0; i < SPN::blockedProcesses.size(); i++){
                        Process tmpProcess = SPN::blockedProcesses.at(i);
                        if (tmpProcess.timeBlocked < lowestTimeBlocked){
                            lowestTimeBlocked = tmpProcess.timeBlocked;
                        }
                    }
                    
                    float idleTime = (lowestTimeBlocked + SPN::block_duration) - simTime;
                    
                    //print stuff for the idle time
                    std::cout   << simTime << "\t"
                                << "(IDLE)\t"
                                << idleTime << "\tI\n";
                    
                    //The idle interval is equal to the length of lowestTimeBlocked
                    simTime += idleTime;
                    
                    //go through blocked vector and unblock any processes that should be unblocked
                    unblockProcesses(simTime);
                }
                
                //if there are no blocked processes, then the simulation is over
                else{
                    std::cout << simTime << "\t(DONE)\t" << getAverageTurnaroundTime() << "\n";
                    break;
                }
            }
        }
    }
    catch(const std::exception &e){
        std::cout << e.what();
    }
}

/**
 * unblockProcesses -   Loops through all blocked processes and determines whether or not to
 *                      "unblock" them (move them back to ready queue)
 *
 * @param simTime           the current simulation time used to compare to each process' timeBlocked
 *
 */
void SPN::unblockProcesses(float simTime){
    for (int i = 0; i < SPN::blockedProcesses.size(); i++){
        Process tmpProcess = SPN::blockedProcesses.at(i);
        
        //if process should be unblocked, move it to ready queue
        if (tmpProcess.timeBlocked + SPN::block_duration <= simTime){
            tmpProcess.timeBlocked = 0.0;
            SPN::readyProcesses.push(tmpProcess);
            SPN::blockedProcesses.erase(SPN::blockedProcesses.begin() + i);
        }
    }
}

/**
 * getAverageTurnaroundTime - Computes average turnaround time of processes using SPN::turnAroundTimes
 *
 * @return                  the average turnaround time of all processes
 *
 */
float SPN::getAverageTurnaroundTime(){
    float size = SPN::turnAroundTimes.size();
    float sum = 0.0;
    for (int i = 0; i < SPN::turnAroundTimes.size(); i++){
        sum += SPN::turnAroundTimes.at(i);
    }
    return sum / size;
}
