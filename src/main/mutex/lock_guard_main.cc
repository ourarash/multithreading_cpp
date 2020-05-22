#include <future>
#include <iostream>
#include <mutex>
#include <numeric>
#include <thread>
#include <vector>

#include "src/lib/utility.h"

// A demo for creating two threads
// Run this using one of the following methods:
//  1. With bazel: bazel run src/main/mutex:lock_gaurd_main
//  2. With plain g++: g++ -std=c++17 -lpthread
//  src/main/mutex/lock_gaurd_main.cc  -I ./
std::mutex g_display_mutex;

void UsingLockGuard() {
  std::lock_guard<std::mutex> guard(g_display_mutex);
  std::thread::id this_id = std::this_thread::get_id();
  std::cout << "My thread id is: " << this_id << std::endl;
}

int main() {
  std::thread t1(UsingLockGuard);
  std::thread t2(UsingLockGuard);

  t1.join();
  t2.join();
}