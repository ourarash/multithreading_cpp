#include <iostream>
#include <thread>

void do_work() {
  // Simulate some work
  std::cout << "Doing work..." << std::endl;
}

void worker(std::stop_token st) {
  while (!st.stop_requested()) {
    do_work(); // your work function
  }
  std::cout << "Worker thread exiting\n";
}

int main() {
  std::jthread t(worker); // passes stop_token to worker

  std::this_thread::sleep_for(std::chrono::seconds(1));

  t.request_stop(); // cooperative cancellation request
                    // automatic join when t goes out of scope
}
