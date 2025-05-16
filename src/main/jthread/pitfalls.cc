#include <thread>

// Don't forget to capture the stop_token in lambdas!
void do_work(std::stop_token st) {
  // Do some work
}

// Don't forget to check the stop_token in loops!
std::jthread t([](std::stop_token st) {
  while (true)
    do_work(); // ❌ ignores stop_token → cannot stop!
});

std::jthread t([](std::stop_token st) {
  while (!st.stop_requested()) { // ✅ stop_token is checked
    do_work();
  }
});

// Don't forget to handle exceptions!
// If an exception is thrown in the thread, it will terminate the program
// unless handled properly.
std::jthread t([](std::stop_token) {
  throw std::runtime_error("oops"); // ❌ terminate
});

std::jthread t([](std::stop_token) {
  try {
    throw std::runtime_error("oops");
  } catch (...) {
    // Handle the exception
  }
});

// Don't forget to pass stop_token as the first argument!
void doWorkWrong(int a, std::stop_token st) { // ❌ error: stop_token must be first
  while (!st.stop_requested()) {
    // Do work
  }
}

void doWorkRight(std::stop_token st, int a) { // ✅ stop_token is first
  while (!st.stop_requested()) {
    // Do work
  }
}
