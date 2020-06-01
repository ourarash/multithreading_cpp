#include <condition_variable>  // std::condition_variable, std::cv_status
#include <future>
#include <iostream>
#include <mutex>  // For std::unique_lock
#include <numeric>
#include <random>
#include <thread>
#include <vector>

#include "src/lib/utility.h"

class channel {
  void Write(int data) {
    std::unique_lock<std::mutex> ul(mutex);
    _req = !_req;
    _data = data;
    ul.unlock();
    cl.notify_one();
    ul.lock();

    cl.wait(ul, [=]() { return ack != _prev_ack; });
    _prev_ack = ack;
  }
  int Read() {
    std::unique_lock<std::mutex> ul(mutex);

    // if blocked, ul.unlock() is automatically called.
    cl.wait(ul, [=]() { return _req != _prev_req; });
    // if unblocks, ul.lock() is automatically called
    int data = _data;
    _prev_req = req;
    _ack = !_ack;
    ul.unlock();
    cl.notify_one();
    ul.lock();
    return data;
  }
  std::mutex _mutex;
  std::condition_variable _cl;
  bool _req = false;
  bool _ack = false;
  bool _prev_req = false;
  bool _prev_ack = false;
  int _data;
};

auto gen = std::bind(std::uniform_int_distribution<>(0, 10),
                     std::default_random_engine());
std::mutex mutex;
std::condition_variable cl;
bool req = false;
bool ack = false;

// A demo for unique_lock: similar to lock_guard, but it can
// be lock and unlock multiple times.

// Run this using one of the following methods:
//  1. With bazel: bazel run src/main/mutex:{THIS_FILE_NAME_WITHOUT_EXTENSION}
//  2. With plain g++: g++ -std=c++17 -lpthread
//  src/main/mutex/{THIS_FILE_NAME}.cc  -I ./
void consumer() {
  int receivedCount = 0;
  bool prevReq = false;
  while (true) {
    std::unique_lock<std::mutex> ul(mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // if blocked, ul.unlock() is automatically called.
    std::cout << "waiting..." << std::endl;
    cl.wait(ul, [=]() { return req != prevReq; });
    // if unblocks, ul.lock() is automatically called
    receivedCount++;
    std::cout << "receivedCount: " << receivedCount << std::endl;

    prevReq = req;
    ack = !ack;
    ul.unlock();
    cl.notify_one();
    ul.lock();
  }
}

void producer() {
  int sentCount = 0;
  bool prevAck = false;

  while (true) {
    std::unique_lock<std::mutex> ul(mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    auto data = gen();
    std::cout << "data: " << data << std::endl;
    sentCount++;
    std::cout << "sentCount: " << sentCount << std::endl;
    req = !req;
    ul.unlock();
    cl.notify_one();
    ul.lock();

    cl.wait(ul, [=]() { return ack != prevAck; });
    prevAck = ack;
  }
}

int main() {
  std::thread t1(consumer);
  std::thread t2(producer);
  t1.join();
  t2.join();
  return 0;
}