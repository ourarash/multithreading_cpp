#ifndef D33FF845_A9B2_4D52_AD68_A63295B8923F
#define D33FF845_A9B2_4D52_AD68_A63295B8923F

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

enum class ChannelStatus { idle, r_pend, w_pend, s12m_pend };

class Channel {
 public:
  Channel() {}
  // Use this if it's a shared 1-to-many channel with multiple receivers.
  Channel(int number_of_receivers)
      : _number_of_receivers(number_of_receivers) {}

  void Write(int data);
  void Write();
  int Read();
  ChannelStatus getStatus() { return _status; }
  bool IsIdle() { return _status == ChannelStatus::idle; }

 private:
  std::mutex _mutex;
  std::condition_variable _cl;
  bool _req = false;
  bool _ack = false;
  bool _handshake_phase = true;
  int _data;
  int _number_of_receivers = 1;
  int _receive_counter = 0;
  ChannelStatus _status = ChannelStatus::idle;
};

class Choice {
 private:
  std::vector<bool> _conditions;

 public:
  Choice(std::initializer_list<bool> &&init_list) noexcept {
    for (auto &entry : init_list) {
      _conditions.push_back(entry);
    }
  }
  int Select() {
    std::vector<int> enable_indices;

    for (int i = 0; i < _conditions.size(); i++) {
      if (_conditions[i]) {
        enable_indices.push_back(i);
      }
    }
    if (enable_indices.empty()) {
      return -1;
    } else {
      auto gen = std::bind(
          std::uniform_int_distribution<>(0, enable_indices.size() - 1),
          std::default_random_engine());
      return enable_indices[gen()];
    }
  }
};

class ForkJoin {
 private:
  std::vector<std::thread> _threads;
  std::vector<std::function<void()>> _funcs;

 public:
  ForkJoin(std::initializer_list<std::function<void()>> &&init_list) noexcept {
    for (auto &entry : init_list) {
      _threads.push_back(std::thread(entry));
    }

    for (auto &t : _threads) {
      if (t.joinable()) {
        t.join();
      }
    }
  }

  ForkJoin(std::vector<std::function<void()>> &init_list) noexcept {
    for (auto &entry : init_list) {
      _threads.push_back(std::thread(entry));
    }

    for (auto &t : _threads) {
      if (t.joinable()) {
        t.join();
      }
    }
  }
};
#endif /* D33FF845_A9B2_4D52_AD68_A63295B8923F */
