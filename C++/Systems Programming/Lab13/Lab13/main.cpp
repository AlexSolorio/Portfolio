//
//  Using multiple threads to speed up array summing algorithm
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include <thread>
#include "Timer.h"

// InitValues - set array values.  Array items are
//   set to the sqrt of their index in the array.
//
// a - pointer to first item to set
// first - index of first array item to set
// last - index of the array item one past the last item to set
void InitValues(double *a, int first, int last) {
  for (int i = first; i < last; ++i) {
    a[i] = sqrt((double) i);
  }
}

// SumValues - sum array values.
//
// a - array to be summed
// sum - return value, the sum of all items in the range
// first - index of first array item to be summed
// last - index of the array item one past the last item to set
void SumValues(const double *a, double *sum, int first, int last) {
  double sumTmp = 0.0;
  for (int i = first; i < last; ++i) {
    sumTmp += a[i];
  }
  *sum = sumTmp;
}

// ThreadedSum - initialize and sum a large array using
//   multiple threads.
//
// numThreads - number of threads to use
// arraySize - size of array to use
double ThreadedSum(int numThreads, size_t arraySize) {
    double sum = 0.0;
    double* array = new double[arraySize];
    
    double *results = new double[numThreads];
    long normalThreadSize = std::ceil(arraySize / numThreads);
    long lastThreadSize = arraySize - ((numThreads-1) * normalThreadSize);
    
    //InitValues(array, 0, (int)arraySize);
    std::thread **initThreads = new std::thread*[numThreads];
    
    //first 7 threads use normalThreadSize
    int i;
    for (i = 0; i < numThreads-1; i++)
    {
        initThreads[i] = new std::thread(InitValues, array, i*normalThreadSize, (i+1)*normalThreadSize);
    }
    
    //last thread uses lastThreadSize
    initThreads[i] = new std::thread(InitValues, array, i*normalThreadSize, (i*normalThreadSize) + lastThreadSize);
    
    for (int x = 0; x < numThreads; x++)
    {
        initThreads[x]->join();
        delete initThreads[x];
        initThreads[x] = 0;
    }
    
    
    std::thread **threads = new std::thread*[numThreads];
    
    //first 7 threads use normalThreadSize
    for (i = 0; i < numThreads-1; i++)
    {
        threads[i] = new std::thread(SumValues, array, &results[i], i*normalThreadSize, (i+1)*normalThreadSize);
    }
    
    //last thread uses lastThreadSize
    threads[i] = new std::thread(SumValues, array, &results[i], i*normalThreadSize, (i*normalThreadSize) + lastThreadSize);
    
    for (int x = 0; x < numThreads; x++)
    {
        threads[x]->join();
        delete threads[x];
        threads[x] = 0;
        sum += results[x];
    }
    
    delete[] array;
    delete[] initThreads;
    delete[] results;
    delete[] threads;
  return sum;
}

int main(int argc, const char * argv[])
{
  // Use the number of hardware threads provided by the CPU(s).
  // If only one hardware thread, use two threads.
  int numThreads = std::thread::hardware_concurrency();
  std::cout << "Hardware threads available: " << numThreads << std::endl;
  if (numThreads < 2) numThreads = 2;  // use at least 2 threads

  // Create a Timer object
  Timer t;

  // Run for each power of 2 from 20 to 26
  for (int power = 20; power <= 26; power++)
  {
    t.StartTimer();
    double sum1 = ThreadedSum(1, 1<<power);
    t.EndTimer();
    std::cout << "Elapsed time: " << t.GetElapsedTime() << ", "
              << "arraySize " << (1<<power) << ", numThreads = 1, sum = "
              << std::setprecision(15) << sum1 << std::endl;

    t.StartTimer();
    double sumN = ThreadedSum(numThreads, 1<<power);
    t.EndTimer();
    std::cout << "Elapsed time: " << t.GetElapsedTime() << ", "
              << "arraySize " <<(1<<power)<< ", numThreads = "
              << numThreads << ", sum = "
              << std::setprecision(15) << sumN << std::endl;
  }
}
