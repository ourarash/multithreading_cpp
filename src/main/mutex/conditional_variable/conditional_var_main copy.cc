#include <condition_variable>  // std::condition_variable, std::cv_status
#include <future>
#include <iostream>
#include <mutex>  // For std::unique_lock
#include <numeric>
#include <thread>
#include <vector>

#include "src/lib/utility.h"
std::mutex mutex;
std::condition_variable cl;
bool ready = false;

// A demo for unique_lock: similar to lock_guard, but it can
// be lock and unlock multiple times.

// Run this using one of the following methods:
//  1. With bazel: bazel run src/main/mutex:{THIS_FILE_NAME_WITHOUT_EXTENSION}
//  2. With plain g++: g++ -std=c++17 -lpthread
//  src/main/mutex/{THIS_FILE_NAME}.cc  -I ./
void consumer() {
  std::unique_lock<std::mutex> ul(mutex);

  cl.wait(ul, []() { return ready; });
  // auto result =
  //     cl.wait_for(ul, std::chrono::milliseconds(100), []() { return ready; });
  // while (!ready) {
  //   std::cout << "waiting..." << std::endl;
  //   cl.wait(ul);
  // }
  std::cout << "ready: " << ready << std::endl;
  std::cout << "I'm free!" << std::endl;
}

void producer() {
  std::this_thread::sleep_for(std::chrono::seconds(4));
  std::unique_lock<std::mutex> ul(mutex);

  ready = true;
  std::cout << "signal once" << std::endl;

  cl.notify_one();
  // ul.unlock();
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "signal again" << std::endl;
  ready = true;

  cl.notify_one();
  // std::this_thread::sleep_for(std::chrono::seconds(2));

  std::cout << "exit" << std::endl;
  // ready = true;
}

int main() {
  std::thread t1(consumer);
  std::thread t2(producer);
  t1.join();
  t2.join();
  return 0;
}