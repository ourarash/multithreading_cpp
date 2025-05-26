#include <exception>
#include <future>
#include <iostream>
#include <thread>

void worker(std::promise<void> p, bool throw_error) {
  try {
    if (throw_error) {
      throw std::runtime_error("Runtime exception!");
    }
    p.set_value(); // signal success
  } catch (...) {
    p.set_exception(std::current_exception()); // signal exception
  }
}

int main() {
  std::promise<void> p;
  std::future<void> f = p.get_future();
  bool throw_error = true; // Toggle this to test both paths

  std::thread t(worker, std::move(p), throw_error);

  try {
    f.get(); // rethrows exception if set_exception was used
    std::cout << "Thread finished without exception.\n";
  } catch (const std::exception &e) {
    std::cout << "Caught: " << e.what() << std::endl;
  }

  t.join();
  return 0;
}
