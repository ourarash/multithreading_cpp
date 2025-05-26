#include <future>
#include <iostream>
#include <stdexcept>
#include <thread>

#include <exception>
#include <future>
#include <iostream>
#include <stop_token>
#include <thread>

int main() {
  std::promise<void> p;
  std::future<void> f = p.get_future();
  bool throw_error = true; // Toggle this to test both paths

  std::jthread t([&]() {
    try {
      if (throw_error) {
        throw std::runtime_error("Runtime exception!");
      }
      p.set_value(); // signal success
    } catch (...) {
      p.set_exception(std::current_exception()); // signal exception
    }
  });

  try {
    f.get(); // rethrows exception if set_exception was used
    std::cout << "Thread finished without exception.\n";
  } catch (const std::exception &e) {
    std::cout << "Caught: " << e.what() << std::endl;
  }

  // No need to call t.join(); jthread auto-joins
  return 0;
}
