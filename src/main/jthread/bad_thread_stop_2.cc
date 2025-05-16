#include <chrono>
#include <future>
#include <iostream>
#include <thread>

void do_work() {
  // Simulate some work
  std::cout << "Doing work..." << std::endl;
}

void worker(std::future<void> stop_signal) {
  while (stop_signal.wait_for(std::chrono::milliseconds(1000)) ==
         std::future_status::timeout) {
    do_work();
  }
}

int main() {
  std::promise<void> exit_signal;
  std::thread t(worker, exit_signal.get_future());

  std::this_thread::sleep_for(std::chrono::seconds(10));
  exit_signal.set_value(); // request stop
  t.join();
}
