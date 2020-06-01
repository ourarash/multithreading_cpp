#ifndef C7A182C8_FBA0_4CFA_BB2F_F9178A1C7683
#define C7A182C8_FBA0_4CFA_BB2F_F9178A1C7683

#include <thread>
#include <vector>

/**
 * A class to create threads in parallel
 */
class ForkJoin {
 private:
  std::vector<std::thread> _threads;
  std::vector<std::function<void()>> _funcs;

 public:
  /**
   * Creates threads from an initialization list of std::functions
   */
  ForkJoin(std::initializer_list<std::function<void()>>&& init_list) noexcept {
    for (auto& entry : init_list) {
      _threads.push_back(std::thread(entry));
    }

    for (auto& t : _threads) {
      if (t.joinable()) {
        t.join();
      }
    }
  }
  /**
   * Creates threads from a vector of std::functions
   */
  ForkJoin(std::vector<std::function<void()>>& init_vector) noexcept {
    for (auto& entry : init_vector) {
      _threads.push_back(std::thread(entry));
    }
    for (auto& t : _threads) {
      if (t.joinable()) {
        t.join();
      }
    }
  }

  /**
   * Creates threads from a vector of std::functions
   */
  // template <typename... Args>
  // ForkJoin(std::vector<std::function<void()>>& v, Args&&... args) noexcept {
  //   (_threads.push_back(args), ...);
  //   for (auto& t : _threads) {
  //     if (t.joinable()) {
  //       t.join();
  //     }
  //   }
  // }

  /**
   * Detaches thread i from the calling thread
   */
  void Detach(int i) {
    _threads[i].detach();
  }
};
#endif /* C7A182C8_FBA0_4CFA_BB2F_F9178A1C7683 */
