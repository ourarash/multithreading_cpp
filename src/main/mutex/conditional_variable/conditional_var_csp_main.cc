#include <iostream>
#include <random>
#include <thread>
#include <vector>

#include "src/lib/csp/csp.h"
#include "src/lib/utility.h"

auto gen = std::bind(std::uniform_int_distribution<>(0, 10),
                     std::default_random_engine());

// A demo for unique_lock: similar to lock_guard, but it can
// be lock and unlock multiple times.

// Run this using one of the following methods:
//  1. With bazel: bazel run src/main/mutex:{THIS_FILE_NAME_WITHOUT_EXTENSION}
//  2. With plain g++: g++ -std=c++17 -lpthread
//  src/main/mutex/{THIS_FILE_NAME}.cc  -I ./
void consumer(Channel &c, int delay_ms) {
  int receivedCount = 0;
  std::thread::id this_id = std::this_thread::get_id();

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));

    int data = c.Read();
    std::cout << "this_id: " << this_id
              << ", receivedCount: " << receivedCount++ << ", data: " << data
              << "\n"
              << std::endl;
  }
}

void producer(Channel &c, int delay_ms) {
  int sentCount = 0;
  std::thread::id this_id = std::this_thread::get_id();

  while (true) {
    auto data = gen();
    std::cout << "sentCount: " << sentCount++ << ", data: " << data
              << std::endl;
    c.Write(data);

    std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
  }
}

int main() {
  Channel c(/*number_of_receivers=2*/ 2);
  std::thread t1(producer, std::ref(c), /*delay_ms=*/200);
  std::thread t2(consumer, std::ref(c), /*delay_ms=*/50);
  std::thread t3(consumer, std::ref(c), /*delay_ms=*/10);

  t1.join();
  t2.join();
  t3.join();
  return 0;
}