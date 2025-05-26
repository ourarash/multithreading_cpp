#include <iostream>
#include <mutex>
#include <stdexcept>

std::mutex mtx;

void worker() {
  try {
    throw std::runtime_error("Boom in thread!");
  } catch (const std::exception &e) {
    std::cerr << "Caught in thread: " << e.what() << '\n';
  }
}
void critical_section() {
  std::lock_guard<std::mutex> lock(mtx);
  std::cout << "Doing something critical..." << std::endl;
  throw std::runtime_error("Error during critical section");
}

int main() {
  try {
    critical_section();
  } catch (const std::exception &e) {
    std::cerr << "Caught: " << e.what() << std::endl;
  }
  return 0;
}