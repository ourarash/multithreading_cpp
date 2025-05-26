#pragma once

#include <map>
#include <mutex>
#include <vector>

template <typename Key, typename Value>
class ThreadSafeMap {
  std::map<Key, std::vector<Value>> store;
  std::mutex mtx;

 public:
  void add(const Key &key, const Value &value) {
    std::lock_guard<std::mutex> lock(mtx);
    store[key].push_back(value);
  }

  std::map<Key, std::vector<Value>> get_data() {
    std::lock_guard<std::mutex> lock(mtx);
    return store;
  }
};