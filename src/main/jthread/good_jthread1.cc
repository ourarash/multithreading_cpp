#include <iostream>
#include <thread>
void worker() { std::cout << "Worker thread running\n"; }

void doSomethingElse() { std::cout << "Main thread doing other work\n"; }

int safeJthread() {
  {
    std::jthread t(worker);
    // Safe: jthread auto-joins on destruction
  }

  // Program continues safely
  doSomethingElse();
}
