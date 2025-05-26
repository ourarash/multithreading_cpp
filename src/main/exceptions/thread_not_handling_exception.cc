#include <iostream>
#include <stdexcept>
#include <thread>

int main() {
  std::thread t([] {
    // This will terminate the program
    throw std::runtime_error("Uncaught exception in thread");
  });
  t.join();
  return 0;
}