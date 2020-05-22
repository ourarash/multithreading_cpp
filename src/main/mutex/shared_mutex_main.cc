#include <future>
#include <iostream>
#include <mutex>  // For std::unique_lock
#include <numeric>
#include <shared_mutex>
#include <thread>
#include <vector>

#include "src/lib/utility.h"

// A demo for creating two threads
// Run this using one of the following methods:
//  1. With bazel: bazel run src/main/mutex:lock_unlock_main
//  2. With plain g++: g++ -std=c++17 -lpthread
//  src/main/mutex/shared_mutex_main.cc  -I ./
std::shared_mutex g_shared_mutex;
int g_shared_var = 1;

int GetValue() {
  std::shared_lock<std::shared_mutex> sl(g_shared_mutex);
  return g_shared_var;
}

void SetValue(int newVal) {
  std::unique_lock<std::shared_mutex> sl(g_shared_mutex);
  g_shared_var = newVal;
}

int main() {
  auto t1 = std::async(GetValue);
  auto t2 = std::async(SetValue, 3);

  int readValue = t1.get();
  t2.get();

  std::cout << "readValue: " << readValue << std::endl;
}