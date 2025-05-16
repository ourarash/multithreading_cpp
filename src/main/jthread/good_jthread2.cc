#include <iostream>
#include <stdexcept>
#include <thread>

void worker() { std::cout << "Worker thread running\n"; }

void doSomethingElse() { std::cout << "Main thread doing other work\n"; }

int safeJthread() {
  std::jthread t(worker);

  // Exception occurs
  throw std::runtime_error("Unexpected error!");

  doSomethingElse(); // Never reached, but no crash
}
