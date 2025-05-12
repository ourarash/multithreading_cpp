/*
 * A demo for creating two threads
 * Run this using one of the following methods:
 *  1. With bazel: bazel run src/main:{THIS_FILE_NAME_WITHOUT_EXTENSION}
 *  2. With plain g++: g++ -std=c++17 -lpthread
 *  src/main/mutex/{THIS_FILE_NAME}.cc  -I ./
 */
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

#include "src/lib/utility.h"

/**
 * Calculates the sum of a range of numbers using multiple threads.
 * Each thread calculates the sum of a portion of the range.
 * The results are then combined to get the total sum.
 */
int main()
{
  const int number_of_threads = 14;
  uint64_t number_of_elements = 1000 * 1000;
  // step is the number of elements each thread will process.
  // For example, if number_of_elements is 1000 and number_of_threads is 10,
  // then each thread will process 100 elements.
  uint64_t step = number_of_elements / number_of_threads;
  std::vector<std::thread> threads;
  std::vector<uint64_t> partial_sums(number_of_threads);

  for (uint64_t i = 0; i < number_of_threads; i++)
  {
    // Creating a lambda function and adding it to the threads.
    threads.push_back(std::thread([i, &partial_sums, step]
                                  {
      // Iterate from i * step to (i + 1) * step and calculate the sum.
      for (uint64_t j = i * step; j < (i + 1) * step; j++) {
        partial_sums[i] += j;
      } }));
  }

  for (std::thread &t : threads)
  {
    if (t.joinable())
    {
      t.join();
    }
  }

  uint64_t total =
      std::accumulate(partial_sums.begin(), partial_sums.end(), uint64_t(0));
  PrintVector(partial_sums);
  std::cout << "total: " << total << std::endl;

  return 0;
}