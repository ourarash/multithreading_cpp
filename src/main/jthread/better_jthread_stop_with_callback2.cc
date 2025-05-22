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
#include <ranges>
#include <thread>

struct SharedState {
  std::condition_variable cv;
  std::mutex mtx;
  bool data_ready{false};
  bool done{false};
};

/**
 * Consumer function that waits for data to be ready or for a stop request.
 * Note: jthread expects the first parameter to be the stop_token.
 */
void consumer(std::stop_token st, SharedState &state) {
  std::stop_callback on_stop(st, [&] {
    std::cout << "Stop requested, waking consumer\n";
    state.cv.notify_one();
  });

  std::unique_lock<std::mutex> lock(state.mtx);
  while (!state.done) {
    state.cv.wait(lock,
                  [&] { return st.stop_requested() || state.data_ready; });
    if (st.stop_requested()) {
      std::cout << "Consumer exiting due to stop\n";
      return;
    }

    if (state.data_ready) {
      std::cout << "Consumer processed data\n";
      state.data_ready = false;
    }
  }
}

void producer(SharedState &state) {
  for (int _ : std::views::iota(0, 5)) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    {
      std::lock_guard<std::mutex> lock(state.mtx);
      state.data_ready = true;
      if (i == 4) {
        done = true; // signal completion
      }
    }
    state.cv.notify_one();
  }
}

int main() {
  SharedState shared;
  std::jthread consumer_thread(consumer, std::ref(shared));
  std::jthread producer_thread(producer, std::ref(shared));
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "Main thread exiting\n";
}
