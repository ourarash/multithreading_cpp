// A demo of producer/consumer using busy waiting on shared memory
// By Ari Saif
// Run this using one of the following methods:
//  1. With bazel:
//      bazel run --cxxopt='-std=c++17' \
//      src/main/conditional_variable:{THIS_FILE_NAME_WITHOUT_EXTENSION}
//  2. With g++:
//      g++ -std=c++17 -lpthread \
//      src/main/conditional_variable/{THIS_FILE_NAME}.cc  -I ./
#include <condition_variable>  // std::condition_variable, std::cv_status
#include <iostream>
#include <mutex>  // For std::unique_lock
#include <random>
#include <thread>

// Create a function to generate a random value between 0 and 10
auto GenRandomValue = std::bind(std::uniform_int_distribution<>(0, 10),
                                std::default_random_engine());

std::mutex g_mutex;
bool g_ready = false;
int g_data = 0;

/**
 * Uses busy waiting on g_ready to get data from producer
 */
void consumer() {
  int receivedCount = 0;

  for (int i = 0; i < 100; i++) {
    std::unique_lock<std::mutex> ul(g_mutex);
    // Busy waiting
    while (!g_ready) {
      ul.unlock();
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      ul.lock();
    }

    std::cout << "got g_data: " << g_data << std::endl;
    g_ready = false;
  }
}

/**
 * Produces data and then sets g_ready to true
 */
void producer() {
  for (int i = 0; i < 100; i++) {
    std::unique_lock<std::mutex> ul(g_mutex);

    // Produce data
    g_data = GenRandomValue();

    // Announce that data is produced
    g_ready = true;

    ul.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    ul.lock();
  }
}

// This example doesn't have conditional variables.
int main() {
  std::thread t1(consumer);
  std::thread t2(producer);
  t1.join();
  t2.join();
  return 0;
}