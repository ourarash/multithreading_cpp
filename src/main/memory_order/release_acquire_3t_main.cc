#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

std::vector<int> data;
std::atomic<int> flag = {0};

void thread_1() {
  data.push_back(42);
  flag.store(1, std::memory_order_release);
}

void thread_2() {
  int expected = 1;
  while (
      !flag.compare_exchange_strong(expected, 2, std::memory_order_acq_rel)) {
    std::cout << "Waiting in t2" << std::endl;
    expected = 1;
  }
}

void thread_3() {
  std::cout << "before load" << std::endl;

  while (flag.load(std::memory_order_acquire) < 2) {
    std::cout << "Waiting in t3" << std::endl;
  }
  assert(data.at(0) == 42);  // will never fire
}

int main() {
  std::thread a(thread_1);
  std::thread b(thread_2);
  std::thread c(thread_3);
  a.join();
  b.join();
  c.join();
}