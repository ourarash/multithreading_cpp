// A demo for incorrectly using shared memory without mutex and locks
// By Ari Saif
// Run this using one of the following methods:
//  1. With bazel:
//      bazel run --cxxopt='-std=c++17' \
//      src/main/mutex:{THIS_FILE_NAME_WITHOUT_EXTENSION}
//  2. With g++:
//      g++ -std=c++17 -lpthread \
//      src/main/mutex/{THIS_FILE_NAME}.cc  -I ./
#include <iostream>
#include <mutex>
#include <numeric>
#include <thread>
#include <vector>

unsigned long g_x;

int main() {
  for (int i = 0; i < 1000; i++) {
    auto t1 = std::thread([]() { g_x = 1; });
    auto t2 = std::thread([]() { g_x = 2; });
    t1.join();
    t2.join();
    std::cout << g_x << ", ";
  }
  std::cout << std::endl;
}