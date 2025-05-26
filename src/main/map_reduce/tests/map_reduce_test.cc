#include "../lib/map_reduce.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <map>
#include <numeric>
#include <string>
#include <vector>

TEST(MapReduceTest, SimpleWordCount) {
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
      2, 2);

  EXPECT_EQ(results["apple"], 3);
  EXPECT_EQ(results["banana"], 2);
  EXPECT_EQ(results["orange"], 1);
  EXPECT_EQ(results["grape"], 3);
}

TEST(MapReduceTest, EmptyInput) {
  std::vector<std::string> data;
  MapReduce<std::string, int> framework;

  auto results = framework.run<int>(
      data,
      [](const std::string &item,
         std::map<std::string, std::vector<int>> &store) {
        store[item].push_back(1);
      },
      [](const std::string &key, const std::vector<int> &values) -> int {
        return std::accumulate(values.begin(), values.end(), 0);
      });

  EXPECT_TRUE(results.empty());
}

TEST(MapReduceTest, SingleElement) {
  std::vector<std::string> data = {"kiwi"};
  MapReduce<std::string, int> framework;

  auto results = framework.run<int>(
      data,
      [](const std::string &item,
         std::map<std::string, std::vector<int>> &store) {
        store[item].push_back(1);
      },
      [](const std::string &key, const std::vector<int> &values) -> int {
        return std::accumulate(values.begin(), values.end(), 0);
      });

  ASSERT_EQ(results.size(), 1);
  EXPECT_EQ(results["kiwi"], 1);
}

TEST(MapReduceTest, CaseInsensitiveWordCount) {
  std::vector<std::string> data = {"Apple", "apple", "APPLE", "Banana",
                                   "BANANA"};
  MapReduce<std::string, int> framework;

  auto results = framework.run<int>(
      data,
      [](const std::string &item,
         std::map<std::string, std::vector<int>> &store) {
        std::string lower = item;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        store[lower].push_back(1);
      },
      [](const std::string &key, const std::vector<int> &values) -> int {
        return std::accumulate(values.begin(), values.end(), 0);
      });

  EXPECT_EQ(results["apple"], 3);
  EXPECT_EQ(results["banana"], 2);
}

TEST(MapReduceTest, CustomReduceSumLengths) {
  std::vector<std::string> data = {"a", "bb", "ccc", "a", "bb"};
  MapReduce<std::string, int> framework;

  auto results = framework.run<int>(
      data,
      [](const std::string &item,
         std::map<std::string, std::vector<int>> &store) {
        store[item].push_back(item.length());
      },
      [](const std::string &key, const std::vector<int> &values) -> int {
        // Sum the lengths for each key
        return std::accumulate(values.begin(), values.end(), 0);
      });

  EXPECT_EQ(results["a"], 2);    // 1 + 1
  EXPECT_EQ(results["bb"], 4);   // 2 + 2
  EXPECT_EQ(results["ccc"], 3);  // 3
}

TEST(MapReduceTest, LargeInput) {
  std::vector<std::string> data;
  for (int i = 0; i < 10000; ++i) {
    data.push_back("x");
  }
  MapReduce<std::string, int> framework;

  auto results = framework.run<int>(
      data,
      [](const std::string &item,
         std::map<std::string, std::vector<int>> &store) {
        store[item].push_back(1);
      },
      [](const std::string &key, const std::vector<int> &values) -> int {
        return std::accumulate(values.begin(), values.end(), 0);
      },
      4, 100);

  EXPECT_EQ(results["x"], 10000);
}

TEST(MapReduceTest, MultipleKeysAndValues) {
  std::vector<std::string> data = {"a", "b", "c", "a", "b", "a"};
  MapReduce<std::string, int> framework;

  auto results = framework.run<int>(
      data,
      [](const std::string &item,
         std::map<std::string, std::vector<int>> &store) {
        store[item].push_back(2);
        store[item].push_back(3);
      },
      [](const std::string &key, const std::vector<int> &values) -> int {
        return std::accumulate(values.begin(), values.end(), 0);
      });

  EXPECT_EQ(results["a"], 15);  // (2+3) * 3
  EXPECT_EQ(results["b"], 10);  // (2+3) * 2
  EXPECT_EQ(results["c"], 5);   // (2+3) * 1
}