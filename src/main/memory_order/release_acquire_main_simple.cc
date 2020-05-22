#include <atomic>
#include <cassert>
#include <chrono>  // std::chrono::seconds
#include <iostream>
#include <string>
#include <thread>

std::atomic<int> a{0};

int data;

// Demo showing release-acquire synchronization
void producer() {
  std::cout << "Before wait" << std::endl;

  std::this_thread::sleep_for(std::chrono::seconds(3));
  std::cout << "Before store" << std::endl;

  a.store(1, std::memory_order_release);
}

void consumer() {
 
  std::cout << "Before load" << std::endl;

  int aa = a.load(std::memory_order_acquire);

  std::cout << "After load" << std::endl;

  std::cout << "aa: " << aa << std::endl;
}

int main() {
  std::thread t1(producer);
  std::thread t2(consumer);
  t1.join();
  t2.join();
}

// std::atomic<int> foo(0);

// void set_foo(int x) {
//   std::this_thread::sleep_for(std::chrono::seconds(3));

//   foo.store(x, std::memory_order_release);  // set value atomically
// }

// void print_foo() {
//   int x;
//   do {
//     std::cout << "Before load" << std::endl;

//     x = foo.load(std::memory_order_acquire);  // get value atomically
//   } while (x == 0);
//   std::cout << "foo: " << x << '\n';
// }

// int main() {
//   std::thread first(print_foo);
//   std::thread second(set_foo, 10);
//   first.join();
//   second.join();
//   return 0;
// }