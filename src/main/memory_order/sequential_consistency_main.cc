#include <iostream>
#include <string>
#include <thread>
#include <vector>

// Global variables
int flag = 0;
int b = 0;
int x = 0, y = 0;
std::string msg, text;

void f1() {
  x = 3;
  y += x;
  msg = "Hello world";
  flag = 1;
  b += flag;
}

void f2() {
  text = "Hi";
  while (flag != 1)
    ;
  text = msg;
}

int main() {
  std::vector<std::thread> threads;

  threads.push_back(std::thread(f1));
  threads.push_back(std::thread(f2));

  for (std::thread &t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }

  std::cout << "msg: " << msg << std::endl;
  std::cout << "b: " << b << ", y: " << y << std::endl;
  std::cout << "text: " << text << std::endl;

  return 0;
}
