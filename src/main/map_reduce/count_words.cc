#include <algorithm>
#include <atomic>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <numeric>
#include <ranges>
#include <stop_token>
#include <thread>
#include <vector>

#include "lib/map_reduce.h"
#include "lib/thread_safe_map.h"

int main() {
  std::vector<std::string> data = {"apple",  "banana", "apple",
                                   "orange", "banana", "apple",
                                   "grape",  "grape",  "grape"};

  MapReduce<std::string, int> framework;

  auto results = framework.run<int>(
      data,
      // Map function
      [](const std::string &item,
         std::map<std::string, std::vector<int>> &store) {
        store[item].push_back(1);
      },
      // Reduce function
      [](const std::string &key, const std::vector<int> &values) -> int {
        return std::accumulate(values.begin(), values.end(), 0);
      },
      /*num_threads=*/4,  // number of threads
      /*batch_size*/ 2    // batch size
                          // Optional combiner can be provided here
  );

  // Print results
  for (const auto &[key, sum] : results) {
    std::cout << key << ": " << sum << std::endl;
  }

  return 0;
}
