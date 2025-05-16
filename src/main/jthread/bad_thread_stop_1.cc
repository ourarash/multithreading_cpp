#include <atomic>
#include <iostream>
#include <thread>

std::atomic<bool> stop_flag{false};
void do_work() {
  // Simulate some work
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
void worker() {
  while (!stop_flag) {
    do_work();
  }
}

int main() {
  std::thread t(worker);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  stop_flag = true; // manually signal to stop
  t.join();
}
