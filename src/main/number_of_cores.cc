#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

#include "src/lib/utility.h"

// A demo for creating two threads
// Run this using one of the following methods:
//  1. With bazel: bazel run src/main:number_of_cores
//  2. With plain g++: g++ -std=c++17 -lpthread src/main/number_of_cores.cc  -I ./
int main() {
  unsigned int c = std::thread::hardware_concurrency();
  std::cout << " number of cores: " << c << std::endl;

  return 0;
}