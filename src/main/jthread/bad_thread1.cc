#include <iostream>
#include <thread>

void worker() {
    std::cout << "Worker thread running\n";
}

void doSomethingElse() {
    std::cout << "Main thread doing other work\n";
}

int problematicThread() {
    {
        std::thread t(worker);
        // Oops! t goes out of scope without join() or detach()
    }
    // Program terminates with std::terminate()
    doSomethingElse();
}
