I created this project in my Operating Systems course in spring 2018. 

In this project, I implemented the Shortest Process Next (SPN) CPU scheduling algorithm. The input file specifies a list of processes, one process per line. 

INPUT FORMAT:
-------------------------------------------------------------------------
The program accepts three command line arguments: the input file name, block duration (the time length that a process is unavailable to run after blocking), and prediction weight (a value between 0.0 and 1.0 used to compute the SPN prediction value).

The input file contains lines sorted in increasing order of arrival time in the system. Each line must have the format:

name arrival_time t0 t1 t2 t3 ...

-"name" is a sequence of non-blank characters representing the name of the process 

-"arrival_time" is the time at which the process arrives in the system

-"t0 t1 t2 ..." is a variable-length list of the actual amount of time the process executes for each of its successive time slots before it blocks. These are the actual run times, not the predicted values that are used for scheduling.
-------------------------------------------------------------------------


OUTPUT FORMAT:

All output is written to standard output. The output is as follows:

-------------------------------------------------------------------------
A single line containing the input file name, the block_duration, and the prediction_weight, separated by spaces.

One line for each interval during which a process is running or the system is idle. The line consists of:
-The current simulation time (starting at 0)
-The process name (or "(IDLE)" if no process is running)
-The length of the interval
-A status code: "B" for blocked, "T" if the process terminated, or "I" for an idle interval
-If the status code is "B", the predicted execution time for the next time slices

After all jobs terminate, a line consisting of the simulation time, the string "(DONE)", and the average turnaround time of all processes is written.
-------------------------------------------------------------------------
