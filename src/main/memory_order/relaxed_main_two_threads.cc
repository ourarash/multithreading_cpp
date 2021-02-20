#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x{0};
std::atomic<int> y{0};
std::atomic<int> z;

void write_x_y() {
  x.store(1, std::memory_order_relaxed);
  y.store(1, std::memory_order_release);
}

void read_y_x() {
  while (!y.load(std::memory_order_relaxed))
    ;
  if (x.load(std::memory_order_relaxed)) {
    ++z;
  }
}

int main() {
  std::thread thread1(write_x_y);
  std::thread thread2(read_y_x);
  thread1.join();
  thread2.join();
}