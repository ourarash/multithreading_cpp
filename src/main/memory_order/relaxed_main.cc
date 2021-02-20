#include <atomic>
#include <string>
#include <thread>
std::atomic<std::string> msg("No message!");
std::atomic<int> flag(0);
std::atomic<bool> c;

void store_msg_flag() {
  msg.store(std::string("Hello world"), std::memory_order_relaxed);
  flag.store(1, std::memory_order_relaxed);
}

void read_flag_msg() {
  while (flag.load(std::memory_order_relaxed) == 0)
    ;
  if (msg.load(std::memory_order_relaxed) != "Hello world") {
    c = true;
  }
}

int main() {
  auto t1 = std::thread(store_msg_flag);
  auto t2 = std::thread(read_flag_msg);
  t1.join();
  t2.join();
  assert(c.load() != true);  // Can fire!
  return 0;
}