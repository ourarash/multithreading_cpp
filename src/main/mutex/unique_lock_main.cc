// A demo for unique_lock: similar to lock_guard, but it can
// be lock and unlock multiple times.
// By Ari Saif
// Run this using one of the following methods:
//  1. With bazel:
//      bazel run --cxxopt='-std=c++17' \
//      src/main/mutex:{THIS_FILE_NAME_WITHOUT_EXTENSION}
//  2. With g++:
//      g++ -std=c++17 -lpthread \
//      src/main/mutex/{THIS_FILE_NAME}.cc  -I ./
#include <future>
#include <iostream>
#include <mutex>  // For std::unique_lock
#include <numeric>
#include <thread>
#include <vector>

#include "src/lib/utility.h"

std::mutex g_mutex;
unsigned long g_counter;

void Incrementer() {
  for (size_t i = 0; i < 100; i++) {
    std::unique_lock<std::mutex> ul(g_mutex);
    g_counter++;
    ul.unlock();
    std::cout << "Doing something non-critical..." << std::endl;
    ul.lock();
  }
}

int main() {
  std::vector<std::thread> threads;

  for (int i = 0; i < 100; i++) {
    threads.push_back(std::thread(Incrementer));
  }

  for (std::thread &t : threads) {
    t.join();
  }
  std::cout << "g_counter: " << g_counter << std::endl;

  assert(g_counter == 100 * 100);
}
