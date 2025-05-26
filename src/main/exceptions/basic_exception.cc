#include "src/lib/logger.h"
#include <iostream>
#include <stdexcept>

void level3() {
  Logger l3("level3");
  throw std::runtime_error("Exception in level3");
}

void level2() {
  Logger l2("level2");
  level3();
}

void level1() {
  Logger l1("level1");
  level2();
}

int main() {
  try {
    level1();
  } catch (const std::exception &e) {
    std::cout << "Caught: " << e.what() << "\n";
  }
}
