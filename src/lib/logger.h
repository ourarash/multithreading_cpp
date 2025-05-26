#include <iostream>

struct Logger {
  std::string name;
  Logger(const std::string &n) : name(n) {
    std::cout << "Constructing " << name << "\n";
  }
  ~Logger() { std::cout << "Destructing " << name << "\n"; }
};