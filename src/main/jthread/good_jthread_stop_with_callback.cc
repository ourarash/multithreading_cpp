#include <atomic>
#include <condition_variable>
#include <exception>
#include <iostream>
#include <mutex>
#include <ranges>
#include <thread>

std::condition_variable cv;
std::mutex mtx;
bool data_ready{false};
bool done{false};

void consumer(std::stop_token st) {
  std::stop_callback on_stop(st, [] {
    std::cout << "Stop requested, waking consumer\n";
    cv.notify_one();
  });

  std::unique_lock<std::mutex> lock(mtx);
  while (!done) {
    cv.wait(lock, [&] { return st.stop_requested() || data_ready; });
    if (st.stop_requested()) {
      std::cout << "Consumer exiting due to stop\n";
      return;
    }

    if (data_ready) {
      std::cout << "Consumer processed data\n";
      data_ready = false;
    }
  }
}

void producer() {
  for (int _ : std::views::iota(0, 5)) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    {
      std::lock_guard<std::mutex> lock(mtx);
      data_ready = true;
      // if (i == 4) {
      //   done = true; // signal completion
      // }
    }
    cv.notify_one();
  }
}

int main() {

  std::jthread consumer_thread(consumer);
  std::jthread producer_thread(producer);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "Main thread exiting\n";
}
