// A demo of producer/Consumer using C++ conditional variable
// By Ari Saif
// Run this using one of the following methods:
//  1. With bazel:
//      bazel run --cxxopt='-std=c++17' \
//      src/main/conditional_variable:{THIS_FILE_NAME_WITHOUT_EXTENSION}
//  2. With g++:
//      g++ -std=c++17 -lpthread \
//      src/main/conditional_variable/{THIS_FILE_NAME}.cc  -I ./
// condition_variable example
#include <condition_variable>  // std::condition_variable
#include <iostream>            // std::cout
#include <mutex>               // std::mutex, std::unique_lock
#include <thread>              // std::thread

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void print_id(int id) {
  std::unique_lock<std::mutex> lck(mtx);
  while (!ready) {
    cv.wait(lck);
  }
  // ...
  std::cout << "thread " << id << '\n';
}

void go() {
  std::unique_lock<std::mutex> lck(mtx);
  ready = true;
  cv.notify_all();
}

int main() {
  std::thread threads[10];
  // spawn 10 threads:
  for (int i = 0; i < 10; ++i) {
    threads[i] = std::thread(print_id, i);
  }

  std::cout << "10 threads ready to race...\n";
  go();  // go!

  for (auto& th : threads) {
    th.join();
  }
  
  return 0;
}