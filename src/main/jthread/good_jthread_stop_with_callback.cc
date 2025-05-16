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

void consumer(std::stop_token st) {
  std::stop_callback on_stop(st, [] {
    std::cout << "Stop requested, waking consumer\n";
    cv.notify_one();
  });

  std::unique_lock<std::mutex> lock(mtx);
  cv.wait(lock, [&] { return data_ready || st.stop_requested(); });

  if (st.stop_requested())
    std::cout << "Consumer exiting due to stop\n";
  else
    std::cout << "Consumer processed data\n";
}

void producer() {
  try {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // simulate exception
    // throw std::runtime_error("Producer error!");

    {
      std::lock_guard<std::mutex> lock(mtx);
      data_ready = true;
      cv.notify_one();
    }
  } catch (...) {
    producer_exception = std::current_exception();
  }
}

int main() {
  std::jthread consumer_thread(consumer);
  std::jthread producer_thread(producer);

  std::this_thread::sleep_for(std::chrono::seconds(2));

  if (producer_exception) {
    try {
      std::rethrow_exception(producer_exception);
    } catch (const std::exception &ex) {
      std::cout << "Caught exception from producer: " << ex.what() << '\n';
      consumer_thread.request_stop();
      std::cout << "Consumer thread requested to stop\n";
    }
  }
}
