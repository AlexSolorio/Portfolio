//
//  This project uses workQueues and threads to speed up the process of finding "perfect numbers" in the range 1 to n
//  A "perfect number" is a positive number that is equal to the sum of all of its positive divisors (excluding the number itself)
//

#include <iostream>
#include <thread>
#include <algorithm>
#include "SharedQueue.h"
#include "Timer.h"

struct Range {
	uint64_t from, to;
};

bool IsPerfectNumber(uint64_t number)
{
	uint64_t limit = number/2;
	uint64_t sum = 0;
	for (uint64_t x = 1; x <= limit; x++)
	{
		if (0 == number%x)
			sum += x;
	}
	return (sum == number);
}

void ThreadWorker(SharedQueue<Range> *workQueue, SharedQueue<uint64_t> *resultQueue)
{
	Range r;
	while (true)
	{
		// read work from queue
		while (workQueue->Remove(r) == false)
		{ std::this_thread::yield(); }
		
		// if from==to, then this is a quit message; we are done
		if (r.from == r.to)
		{
			break;
		}

    // compute whether each number in Range is a perfect number
    // and put perfect numbers into the result queue
		for (uint64_t x = r.from; x < r.to; x++)
		{
			if (IsPerfectNumber(x))
				resultQueue->Add(x);
		}
		
	}
}

void FindPerfectNumbers(uint64_t maxValue, int workSize, unsigned numThreads)
{
	SharedQueue<Range> workQueue;
	SharedQueue<uint64_t> resultQueue;
	Timer t;
	t.StartTimer();
	
	std::thread **threads;
	threads = new std::thread*[numThreads];
	for (int x = 0; x < numThreads; x++)
	{
		threads[x] = new std::thread(ThreadWorker, &workQueue, &resultQueue);
	}
	
	for (uint64_t x = 1; x < maxValue; x+=workSize)
	{
		workQueue.Add({x, std::min(x+workSize, maxValue)});
	}
	
	for (int x = 0; x < numThreads; x++)
	{
		workQueue.Add({0,0});
	}
	
	for (int x = 0; x < numThreads; x++)
	{
		threads[x]->join();
		delete threads[x];
	}
	delete [] threads;
	
    print_lock.lock();
	std::cout << "Perfect numbers:" << std::endl;
    print_lock.unlock();
	while (!resultQueue.IsEmpty())
	{
		uint64_t result;
        if (resultQueue.Remove(result)){
            print_lock.lock();
			std::cout << result << std::endl;
            print_lock.unlock();
        }
	}
    print_lock.lock();
	std::cout << "Finding perfect numbers from 1 to " << maxValue << " took " << t.EndTimer() << " seconds.\n";
	std::cout << numThreads << " threads; work unit " << workSize << " items." << std::endl << "\n";
    print_lock.unlock();
}

int main(int argc, const char * argv[])
{
	unsigned num_threads = std::thread::hardware_concurrency();
	
	FindPerfectNumbers(10000, 10000, 1);
	
	FindPerfectNumbers(10000, 1, 1);
	FindPerfectNumbers(10000, 1, num_threads);
	FindPerfectNumbers(10000, 100, num_threads);
	
	FindPerfectNumbers(100000, 100, num_threads);
	
	// FindPerfectNumbers(1000000, 100, num_threads);
	// This will find the next perfect number, but will take too long in practice
	// FindPerfectNumbers(34000000, 1000, num_threads);
	
	return 0;
}

/**
 * Report here:
 *
 * What are the runtimes for each of the 5 calls above?
 
 The runtimes for the 5 calls above are: 0.2561s, 0.2544s, 0.0550s, 0.0444s, and 4.2705s
 
 * Can you explain the differences in runtime for computing
 * perfect numbers within the first 10,000 elements?
 
 For the first and second computations, one thread is doing all the work, with the only difference being how 
 large the workSize is. When the workSize is 10000, the one thread is doing one large job from the workQueue, 
 whereas when the workSize is 1, the one thread is doing 10000 small jobs from the workQueue. Either way it's
 doing approximately the same number of computations, so the first two runtimes are similar. When you use multiple
 threads, however, the total time for the computation goes down, because in my case I have 8 threads executing 
 different areas of the computation all at once. Although the run time for the third case is similar to that of
 the first two, when you use threads AND make the workSize a larger number than 1 (100) the computation gets 
 far faster.
 
 
 */
