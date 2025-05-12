
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

/**
 * Calculates the sum of a range of numbers using multiple threads.
 * Each thread calculates the sum of a portion of the range.
 * The results are then combined to get the total sum.
 */
int main() {
  const uint64_t number_of_threads = 14;
  constexpr uint64_t number_of_elements = 1'000'000ULL;
  constexpr uint64_t step = number_of_elements / number_of_threads;

  std::vector<uint64_t> partial_sums(number_of_threads, 0);
  {
    std::vector<std::jthread> threads;
    for (uint64_t i = 0; i < number_of_threads; i++) {
      threads.emplace_back([i, &partial_sums]() {
        uint64_t local_sum = 0;
        uint64_t start = i * step;
        uint64_t end =
            (i == number_of_threads - 1) ? number_of_elements : (i + 1) * step;
        for (uint64_t j = start; j < end; j++)
          local_sum += j;
        partial_sums[i] = local_sum;
      });
    }
  }

  for (uint64_t i = 0; i < number_of_threads; i++)
    std::cout << "partial_sums[" << i << "] = " << partial_sums[i] << std::endl;

  uint64_t total =
      std::accumulate(partial_sums.begin(), partial_sums.end(), uint64_t(0));
  std::cout << "total: " << total << std::endl;
}
