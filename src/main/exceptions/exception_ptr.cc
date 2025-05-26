#include <exception>
#include <iostream>
#include <stdexcept>
#include <thread>

void worker(std::exception_ptr &eptr) {
  try {
    throw std::runtime_error("Something failed in worker");
  } catch (...) {
    eptr = std::current_exception();
  }
}

int main() {
  std::exception_ptr eptr;
  std::thread t(worker, std::ref(eptr));
  t.join();

  if (eptr) {
    try {
      std::rethrow_exception(eptr);
    } catch (const std::exception &e) {
      std::cerr << "Caught from worker: " << e.what() << std::endl;
    }
  }
  return 0;
}