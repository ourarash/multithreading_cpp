#include <iostream>
#include <mutex>  // For std::unique_lock
#include <random>
#include <thread>
#include <vector>

std::mutex mutex;
int x = 0, y = 0;

int main() {
  std::thread A{[] {
    x = 1;
    std::lock_guard<std::mutex> lg(std::mutex);
    y = 0;
  }};
  std::thread B{[] {
    std::lock_guard<std::mutex> lg(std::mutex);
    y = x + 2;
  }};

  A.join();
  B.join();
  std::cout << "x: " << x << std::endl;
  std::cout << "y: " << y << std::endl;
}