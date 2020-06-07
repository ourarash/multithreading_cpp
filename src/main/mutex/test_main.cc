#include <iostream>
#include <mutex>  // For std::unique_lock
#include <random>
#include <thread>
#include <vector>

using namespace std;
mutex mA, mB, coutMutex;
bool fA = false, fB = false;

int main() {
  thread A{[] {
    lock_guard<mutex> lock{mA};
    fA = true;
  }};
  thread B{[] {
    lock_guard<mutex> lock{mB};
    fB = true;
  }};
  thread C{[] {  // reads fA, then fB
    mA.lock();
    const auto _1 = fA;
    mA.unlock();
    mB.lock();
    const auto _2 = fB;
    mB.unlock();
    lock_guard<mutex> lock{coutMutex};
    cout << "Thread C: fA = " << _1 << ", fB = " << _2 << endl;
  }};
  thread D{[] {  // reads fB, then fA (i. e. vice versa)
    mB.lock();
    const auto _3 = fB;
    mB.unlock();
    mA.lock();
    const auto _4 = fA;
    mA.unlock();
    lock_guard<mutex> lock{coutMutex};
    cout << "Thread D: fA = " << _4 << ", fB = " << _3 << endl;
  }};
  A.join();
  B.join();
  C.join();
  D.join();
}