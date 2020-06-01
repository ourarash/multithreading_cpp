#include <cassert>
#include <iostream>
#include <random>
#include <sstream>
#include <thread>
#include <vector>

#include "src/lib/csp/csp.h"
#include "src/lib/utility.h"
/** Thread safe cout class
 * Exemple of use:
 *    PrintThread{} << "Hello world!" << std::endl;
 */
class PrintThread : public std::ostringstream {
 public:
  PrintThread() = default;

  ~PrintThread() {
    std::lock_guard<std::mutex> guard(_mutexPrint);
    std::cout << this->str();
  }

 private:
  static std::mutex _mutexPrint;
};

std::mutex PrintThread::_mutexPrint{};
auto gen = std::bind(std::uniform_int_distribution<>(0, 10),
                     std::default_random_engine());

// A demo for unique_lock: similar to lock_guard, but it can
// be lock and unlock multiple times.

// Run this using one of the following methods:
//  1. With bazel: bazel run src/main/mutex:{THIS_FILE_NAME_WITHOUT_EXTENSION}
//  2. With plain g++: g++ -std=c++17 -lpthread
//  src/main/mutex/{THIS_FILE_NAME}.cc  -I ./

void fork(Channel &left, Channel &right, int delay_ms, int number) {
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    Choice c{!left.IsIdle(), !right.IsIdle()};
    int select = c.Select();
    // if (select != -1) {
    //   PrintThread{} << number << ". left.IsIdle(): " << left.IsIdle()
    //                 << ", right.IsIdle(): " << right.IsIdle() << std::endl;
    //   PrintThread{} << number << ".select: " << select << std::endl;
    // }
    switch (select) {
      case 0:
        left.Read();
        PrintThread{} << number << ". Left picked me up!" << std::endl;
            // std::this_thread::sleep_for(std::chrono::milliseconds(100));

        left.Read();
        PrintThread{} << number << ". Left put me down!" << std::endl;
        break;
      case 1:
        right.Read();
        PrintThread{} << number << ". Right picked me up!" << std::endl;
        right.Read();
        PrintThread{} << number << ". Right put me down!" << std::endl;

        break;
    }
  }
}

void phil(Channel &left, Channel &right, int delay_ms, int number) {
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    PrintThread{} << number << ". Thinking!" << std::endl;
    left.Write();
    PrintThread{} << number << ". left up" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    right.Write();
    PrintThread{} << number << ". Eating!" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    left.Write();
    right.Write();
    // ForkJoin f1{[&]() {
    //               left.Write();
    //               PrintThread{} << number << ". left up" << std::endl;
    //             },

    //             [&]() {
    //               right.Write();
    //               PrintThread{} << number << ". right up" << std::endl;
    //             }};

    // PrintThread{} << number << ". Eating!" << std::endl;

    // ForkJoin f2{[&]() { left.Write(); },

    //             [&]() { right.Write(); }};
    // up.Write();
  }
}

// void butler(Channel &up, Channel &down, int delay_ms) {
//   // int seated =
//   while (true) {
//     std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
//     PrintThread{} << "Thinking!" << std::endl;
//     down.Write();
//     PrintThread{} << "Seated!" << std::endl;
//     ForkJoin f1{[&]() { left.Write(); },

//                 [&]() { right.Write(); }};

//     PrintThread{} << "Eating!" << std::endl;

//     ForkJoin f2{[&]() { left.Write(); },

//                 [&]() { right.Write(); }};
//     up.Write();
//   }
// }

int main() {
  const int N = 3;
  PrintThread{} << "Start!" << std::endl;
  std::vector<Channel> left(N);
  std::vector<Channel> right(N);

  std::vector<std::function<void()>> processes;

  for (int i = 0; i < N; i++) {
    processes.push_back(
        std::bind(fork, std::ref(right[i]), std::ref(left[(i + 1) % N]), 0, i));
  }

  for (int i = 0; i < N; i++) {
    processes.push_back(
        std::bind(phil, std::ref(left[i]), std::ref(right[i]), 1000, i));
  }

  ForkJoin f(processes);
}