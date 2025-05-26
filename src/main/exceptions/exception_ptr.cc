#include <exception>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <ranges>
#include <numeric>

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

  int sum = std::ranges::fold_left(values, 0, std::plus{});

  return 0;
}