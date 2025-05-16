#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <ranges>
#include <thread>

std::condition_variable cv;
std::mutex mtx;
bool data_ready = false;
bool done = false;

void consumer() {
  std::unique_lock<std::mutex> lock(mtx);
  while (!done) {
    cv.wait(lock, [&] { return data_ready || done; });

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
  std::thread consumer_thread(consumer);
  std::thread producer_thread(producer);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  producer_thread.join();
  consumer_thread.join();
}
