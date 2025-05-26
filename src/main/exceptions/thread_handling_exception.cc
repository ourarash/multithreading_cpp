#include <iostream>
#include <stdexcept>
#include <thread>

int main() {
  std::thread t([] {
    try {
      throw std::runtime_error("Boom in thread!");
    } catch (const std::exception &e) {
      std::cerr << "Caught in thread: " << e.what() << '\n';
    }
  });
  t.join();
  std::cout << "Main thread continues safely\n";
  return 0;
}
