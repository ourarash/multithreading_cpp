#include <chrono>
#include <condition_variable>
#include <iostream>
#include <thread>

std::mutex mtx;
std::condition_variable cv;

void do_work() {
  std::cout << "Working...\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void worker(std::stop_token st) {
  std::unique_lock lock(mtx);
  while (!st.stop_requested()) {
    do_work();

    // Block for 500ms or until stop is requested
    cv.wait_for(lock, std::chrono::milliseconds(500),
                [&] { return st.stop_requested(); });
  }

  std::cout << "Worker thread exiting\n";
}

int main() {
  std::jthread t(worker); // passes stop_token automatically

  std::this_thread::sleep_for(std::chrono::seconds(2));
  t.request_stop(); // cooperative cancellation
}
