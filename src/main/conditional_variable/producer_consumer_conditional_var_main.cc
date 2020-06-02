#include <condition_variable>  // std::condition_variable, std::cv_status
#include <future>
#include <iostream>
#include <mutex>  // For std::unique_lock
#include <numeric>
#include <random>
#include <thread>
#include <vector>

std::mutex g_mutex;
std::condition_variable g_cv;
bool g_ready = false;
int g_data = 0;
// Create a function to generate a random value between 0 and 10
auto GenRandomValue = std::bind(std::uniform_int_distribution<>(0, 10),
                                std::default_random_engine());

// A demo for unique_lock: similar to lock_guard, but it can
// be lock and unlock multiple times.

// Run this using one of the following methods:
//  1. With bazel: bazel run src/main/mutex:{THIS_FILE_NAME_WITHOUT_EXTENSION}
//  2. With plain g++: g++ -std=c++17 -lpthread
//  src/main/mutex/{THIS_FILE_NAME}.cc  -I ./
void consumer() {
  for (int i = 0; i < 100; i++) {
    std::unique_lock<std::mutex> ul(g_mutex);

    // if blocked, ul.unlock() is automatically called.
    // if unblocked, ul.lock() is automatically called.
    g_cv.wait(ul, []() { return g_ready; });

    std::cout << "got g_data: " << g_data << std::endl;
  }
}

void producer() {
  for (int i = 0; i < 100; i++) {
    std::unique_lock<std::mutex> ul(g_mutex);

    // Produce data
    g_data = GenRandomValue();

    // Announce that data is produced
    g_ready = true;
    ul.unlock();
    g_cv.notify_one();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    ul.lock();
  }
}

int main() {
  std::thread t1(consumer);
  std::thread t2(producer);
  t1.join();
  t2.join();
  return 0;
}