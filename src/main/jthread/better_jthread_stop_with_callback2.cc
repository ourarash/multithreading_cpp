/**
 * This version uses a future and promise to not only signal the completion but
 * also propagate exceptions from the producer to the main thread.
 * Also, it uses a shared state instead of global variables.
 */
#include <condition_variable>
#include <exception>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>

struct SharedState {
  std::mutex mtx;
  std::condition_variable cv;
  bool data_ready{false};
};

/**
 * Consumer function that waits for data to be ready or for a stop request.
 * Note: jthread expects the first parameter to be the stop_token.
 */
void consumer(std::stop_token st, SharedState &state) {
  std::stop_callback on_stop(st, [&] {
    std::cout << "Stop requested → waking consumer\n";
    state.cv.notify_one();
  });

  std::unique_lock<std::mutex> lock(state.mtx);
  state.cv.wait(lock, [&] { return state.data_ready || st.stop_requested(); });

  if (st.stop_requested())
    std::cout << "Consumer exiting due to stop\n";
  else
    std::cout << "Consumer processed data\n";
}

void producer(SharedState &state, std::promise<void> &producer_promise) {
  try {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    throw std::runtime_error("Producer error!");

    {
      std::lock_guard<std::mutex> lock(state.mtx);
      state.data_ready = true;
    }
    state.cv.notify_one();
    producer_promise.set_value();
  } catch (...) {
    producer_promise.set_exception(std::current_exception());
  }
}

int main() {
  SharedState shared;
  std::promise<void> producer_promise;
  std::future<void> producer_future = producer_promise.get_future();

  std::jthread consumer_thread(consumer, std::ref(shared));
  std::jthread producer_thread(producer, std::ref(shared),
                               std::ref(producer_promise));

  try {
    producer_future.get(); // blocks and propagates exception
  } catch (const std::exception &ex) {
    std::cout << "Caught exception from producer: " << ex.what() << '\n';
    consumer_thread.request_stop();
    std::cout << "Sent stop request to consumer\n";
  }
}
