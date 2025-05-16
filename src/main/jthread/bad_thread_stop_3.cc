#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx;
std::condition_variable cv;
bool stop_requested = false;

void worker() {
  std::unique_lock<std::mutex> lock(mtx);
  while (!stop_requested) {
    cv.wait_for(lock, std::chrono::milliseconds(100)); // periodic wake-up
    do_work();
  }
  std::cout << "Worker thread exiting\n";
}

int main() {
  std::thread t(worker);

  std::this_thread::sleep_for(std::chrono::seconds(1));
  {
    std::lock_guard<std::mutex> lock(mtx);
    stop_requested = true;
  }
  cv.notify_one(); // wake up thread to exit

  t.join();
}
