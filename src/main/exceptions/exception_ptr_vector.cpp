#include <exception>
#include <iostream>
#include <thread>
#include <vector>

std::vector<std::exception_ptr> exceptions;

void worker(int id) {
  try {
    if (id == 1)
      throw std::runtime_error("Worker failed");
  } catch (...) {
    exceptions[id] = std::current_exception();
  }
}

int main() {
  int n = 3;
  exceptions.resize(n);
  std::vector<std::thread> threads;

  for (int i = 0; i < n; ++i)
    threads.emplace_back(worker, i);

  for (auto &t : threads)
    t.join();

  for (int i = 0; i < n; ++i) {
    if (exceptions[i]) {
      try {
        std::rethrow_exception(exceptions[i]);
      } catch (const std::exception &e) {
        std::cout << "Thread " << i << ": " << e.what() << '\n';
      }
    }
  }
}
