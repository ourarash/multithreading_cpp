#include <atomic>
#include <condition_variable>
#include <exception>
#include <iostream>
#include <mutex>
#include <thread>

std::condition_variable cv;
std::mutex mtx;
bool data_ready{false};
std::exception_ptr producer_exception;

void consumer() {
  std::unique_lock<std::mutex> lock(mtx);
  cv.wait(lock, [&] { return data_ready; });
  std::cout << "Consumer processed data\n";
}

void producer() {
  std::this_thread::sleep_for(std::chrono::seconds(1));
  // simulate exception
  throw std::runtime_error("Producer error!");

  {
    std::lock_guard<std::mutex> lock(mtx);
    data_ready = true;
  }
  cv.notify_one();
}

int main() {
  std::thread consumer_thread(consumer);
  std::thread producer_thread(producer);

  producer_thread.join();
  consumer_thread.join();
}
