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

#include "thread_safe_map.h"

// Generic MapReduce framework supporting customizable map, combine, reduce
// Supports multithreaded map and reduce stages with optional combiner phase
template <typename Key, typename Value>
class MapReduce {
 public:
  // MapFunction:
  //   param 1: const string&: The raw input record (e.g., a line from a file)
  //   param 2: map<Key, vector<Value>>&: Per-thread local store to emit
  //   intermediate key-value pairs
  using MapFunction = std::function<void(const std::string &,
                                         std::map<Key, std::vector<Value>> &)>;

  // CombineFunction:
  //   param 1: vector of per-thread maps (input from all map threads)
  //   param 2: shared thread-safe global store to merge into
  using CombineFunction =
      std::function<void(std::vector<std::map<Key, std::vector<Value>>> &,
                         ThreadSafeMap<Key, Value> &)>;

  // ReduceFunction:
  //   param 1: const Key&: The key to reduce
  //   param 2: const vector<Value>&: All values associated with the key
  //   returns: ResultType - the reduced result for the key
  template <typename ResultType>
  using ReduceFunction =
      std::function<ResultType(const Key &, const std::vector<Value> &)>;

  // Executes the MapReduce job over the given input strings
  // Supports optional custom combiner and batch size
  template <typename ResultType>
  std::map<Key, ResultType> run(
      const std::vector<std::string> &inputs, MapFunction map_func,
      ReduceFunction<ResultType> reduce_func,
      size_t num_threads = std::thread::hardware_concurrency(),
      size_t batch_size = 1, CombineFunction combiner = nullptr) {
    ThreadSafeMap<Key, Value> store;
    std::atomic<size_t> map_progress(0);
    std::atomic<size_t> reduce_progress(0);

    size_t total = inputs.size();
    size_t chunk_size = std::ceil((double)total / num_threads);

    // Each thread gets its own local map to avoid contention
    std::vector<std::map<Key, std::vector<Value>>> localMaps(num_threads);

    // Map phase
    {
      std::vector<std::jthread> mappers;
      for (size_t t = 0; t < num_threads; t++) {
        size_t start = t * chunk_size;
        size_t end = std::min(start + chunk_size, total);
        if (start >= end) {
          break;
        }
        size_t s = start, e = end, tid = t;

        // Each thread maps its chunk into its own local map
        mappers.emplace_back([&, s, e, tid]() {
          for (size_t i = s; i < e; i += batch_size) {
            size_t batch_end = std::min(i + batch_size, e);
            for (size_t j = i; j < batch_end; j++) {
              map_func(inputs[j], localMaps[tid]);
            }
            map_progress += batch_end - i;
          }
        });
      }
    }  // jthreads auto-join on scope exit

    // Optional combiner: merges local maps into global store
    if (combiner) {
      combiner(localMaps, store);
    } else {
      // Default combine: simple merge with locking
      for (const auto &local : localMaps) {
        for (const auto &[key, values] : local) {
          for (const auto &v : values) {
            store.add(key, v);
          }
        }
      }
    }

    // Reduce phase: partition keys and reduce in parallel
    auto data = store.get_data();
    std::vector<std::pair<Key, std::vector<Value>>> items(data.begin(),
                                                          data.end());
    size_t reduce_chunk = std::ceil((double)items.size() / num_threads);

    std::map<Key, ResultType> results;
    std::mutex results_mtx;

    {
      std::vector<std::jthread> reducers;
      for (size_t t = 0; t < num_threads; t++) {
        size_t start = t * reduce_chunk;
        size_t end = std::min(start + reduce_chunk, items.size());
        if (start >= end) {
          break;
        }

        // Each thread reduces a slice of key-value groups
        reducers.emplace_back([&, start, end]() {
          for (size_t i = start; i < end; i++) {
            ResultType result = reduce_func(items[i].first, items[i].second);
            {
              std::lock_guard<std::mutex> lock(results_mtx);
              results[items[i].first] = result;
            }
            reduce_progress++;
          }
        });
      }
    }  // jthreads auto-join on scope exit

    return results;
  }
};
