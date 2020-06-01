#include <future>
#include <iostream>
#include <mutex>
#include <numeric>
#include <thread>
#include <vector>

#include "src/lib/utility.h"

// A demo for creating two threads
// Run this using one of the following methods:
//  1. With bazel: bazel run src/main/mutex:{THIS_FILE_NAME_WITHOUT_EXTENSION}
//  2. With plain g++: g++ -std=c++17 -lpthread
//  src/main/mutex/{THIS_FILE_NAME}  -I ./
std::mutex g_mutex1, g_mutex2;
unsigned long g_counter;

void Incrementer_Bad1() {
  for (size_t i = 0; i < 100; i++) {
    std::lock_guard<std::mutex> lock1(g_mutex1);
    std::lock_guard<std::mutex> lock2(g_mutex2);
    g_counter++;
  }
}

void Incrementer_Bad2() {
  for (size_t i = 0; i < 100; i++) {
    std::lock_guard<std::mutex> lock2(g_mutex2);
    std::lock_guard<std::mutex> lock1(g_mutex1);
    g_counter++;
  }
}

void Incrementer_Better() {
  for (size_t i = 0; i < 100; i++) {
    // lock both mutexes without deadlock
    std::lock(g_mutex1, g_mutex2);

    // make sure both already-locked mutexes are unlocked at the end of scope
    std::lock_guard<std::mutex> lock1(g_mutex1, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(g_mutex2, std::adopt_lock);
    g_counter++;
  }
}

void Incrementer_Best() {
  for (size_t i = 0; i < 100; i++) {
    std::scoped_lock scoped_lock_name(g_mutex1, g_mutex2);
    g_counter++;
  }
}

int main() {
  std::vector<std::thread> threads;

  for (int i = 0; i < 100; i++) {
    // threads.push_back(std::thread(Incrementer_Bad1));
    // threads.push_back(std::thread(Incrementer_Bad2));
    threads.push_back(std::thread(Incrementer_Best));
  }

  for (std::thread &t : threads) {
    t.join();
  }
  std::cout << "g_counter: " << g_counter << std::endl;

  assert(g_counter == 100 * 100);
}
