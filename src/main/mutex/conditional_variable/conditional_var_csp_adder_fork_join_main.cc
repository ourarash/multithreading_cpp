#include <cassert>
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
    // std::cout << "this_id: " << this_id
    //           << ", receivedCount: " << receivedCount++ << ", data: " << data
    //           << "\n"
    //           << std::endl;
  }
}

void adder(Channel &in1, Channel &in2, Channel &out, int delay_ms) {
  std::thread::id this_id = std::this_thread::get_id();

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));

    int sum = 0;

    sum += in1.Read();
    sum += in2.Read();

    out.Write(sum);
  }
}

void producer(Channel &c, int delay_ms) {
  while (true) {
    auto data = gen();
    c.Write(data);

    std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
  }
}

void monitor(Channel &in1, Channel &in2, Channel &out) {
  while (true) {
    int sum = 0;
    sum += in1.Read();
    sum += in2.Read();

    int actual_sum = out.Read();

    std::cout << "sum: " << sum << ", actual_sum: " << actual_sum << std::endl;
    assert(sum == actual_sum);
    std::cout << "Assert passed!" << std::endl;
  }
}

int main() {
  Channel op1(2);
  Channel op2(2);
  Channel result(2);

  ForkJoin f{
      std::bind(producer, std::ref(op1), /*delay_ms=*/200),
      std::bind(producer, std::ref(op2), /*delay_ms=*/200),
      std::bind(adder, std::ref(op1), std::ref(op2), std::ref(result),
                /*delay_ms=*/50),
      std::bind(consumer, std::ref(result),
                /*delay_ms=*/500),
      std::bind(monitor, std::ref(op1), std::ref(op2), std::ref(result))};

  return 0;
}