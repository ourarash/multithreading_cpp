#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
#include <future>

#include "src/lib/utility.h"

// A demo for creating two threads
// Run this using one of the following methods:
//  1. With bazel: bazel run src/main:vector_of_threads_main
//  2. With plain g++: g++ -std=c++17 -lpthread
//  src/main/vector_of_threads_main.cc  -I ./
int main() {
  const int number_of_threads = 10;
  uint64_t number_of_elements = 1000 * 1000 * 1000;
  uint64_t step = number_of_elements / number_of_threads;
  std::vector<std::future<uint64_t>> tasks;

  for (uint64_t i = 0; i < number_of_threads; i++) {
    tasks.push_back(std::async(GetRangeSum, i * step, (i + 1) * step));
  }
  
  uint64_t total = 0;
  for (auto &t : tasks) {
    total += t.get();
  }

  std::cout << "total: " << total << std::endl;

  return 0;
}