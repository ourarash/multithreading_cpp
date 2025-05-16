#include <atomic>
#include <condition_variable>
#include <exception>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>

// This version uses a future and promise to not only signal the completion but
// also propagate exceptions from the producer to the main thread.

std::condition_variable cv;
std::mutex mtx;
bool data_ready{false};

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

void producer(std::promise<void> &done_promise) {
  try {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    throw std::runtime_error("Producer error!");

    {
      std::lock_guard<std::mutex> lock(mtx);
      data_ready = true;
      cv.notify_one();
    }
    done_promise.set_value();
  } catch (...) {
    done_promise.set_exception(std::current_exception());
  }
}

int main() {
  std::promise<void> producer_done;
  std::future<void> done_future = producer_done.get_future();

  std::jthread consumer_thread(consumer);
  std::jthread producer_thread(producer, std::ref(producer_done));

  try {
    done_future.get(); // ✅ blocks and rethrows exception if any
  } catch (const std::exception &ex) {
    std::cout << "Caught exception from producer: " << ex.what() << '\n';
    consumer_thread.request_stop();
    std::cout << "Consumer thread requested to stop\n";
  }
}
