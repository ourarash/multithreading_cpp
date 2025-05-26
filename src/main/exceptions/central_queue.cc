#include <condition_variable>
#include <exception>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>
#include <vector>

class ExceptionChannel {
  std::queue<std::exception_ptr> q;
  std::mutex mtx;

public:
  void push(std::exception_ptr e) {
    std::lock_guard<std::mutex> lock(mtx);
    q.push(e);
  }

  std::optional<std::exception_ptr> pop() {
    std::lock_guard<std::mutex> lock(mtx);
    if (q.empty())
      return std::nullopt;
    auto e = q.front();
    q.pop();
    return e;
  }

  bool empty() {
    std::lock_guard<std::mutex> lock(mtx);
    return q.empty();
  }
};

void worker(int id, ExceptionChannel &channel) {
  try {
    if (id % 2 == 0) {
      throw std::runtime_error("Failure from thread " + std::to_string(id));
    }
  } catch (...) {
    channel.push(std::current_exception());
  }
}

int main() {
  ExceptionChannel channel;

  {
    std::vector<std::jthread> jthreads;

    for (int i = 0; i < 10; ++i) {
      jthreads.emplace_back(worker, i, std::ref(channel));
    }
  }

  while (auto eptr = channel.pop()) { // pop() returns nullopt when empty
    try {
      std::rethrow_exception(*eptr);
    } catch (const std::exception &e) {
      std::cout << "Caught: " << e.what() << '\n';
    }
  }
}
