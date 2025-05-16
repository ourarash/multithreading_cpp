#include <iostream>
#include <thread>

void worker() { std::cout << "Worker thread running\n"; }

void doSomethingElse() { std::cout << "Main thread doing other work\n"; }

void problematicThread() {
  std::thread t(worker);
  // Simulate an exception before calling t.join()
  throw std::runtime_error("Unexpected error!");
  t.join(); // Never reached → std::terminate() called
  doSomethingElse();
}
