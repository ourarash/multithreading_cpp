#include <algorithm>
#include <cstdint>
#include <execution>
#include <gtest/gtest.h>
#include <iostream>
#include <numeric>
#include <ranges>
#include <thread>
#include <vector>

/**
 * Calculates the sum of a range of numbers using multiple threads.
 * Each thread calculates the sum of a portion of the range.
 * The results are then combined to get the total sum.
 */
void compute_partial_sum(std::size_t num_threads, std::size_t index,
                         std::size_t chunk_size, std::size_t total_elements,
                         std::vector<std::uint64_t> &partial_sums) {

  const auto start = index * chunk_size;
  const auto end = (index == num_threads - 1)
                       ? total_elements
                       : std::min((index + 1) * chunk_size, total_elements);

  auto range = std::views::iota(start, end);
  partial_sums[index] =
      std::ranges::fold_left(range, std::uint64_t{0}, std::plus{});
}

/**
 * Computes the total sum of integers from 0 to total_elements - 1
 * using multiple threads.
 */
std::uint64_t compute_total_sum(std::size_t num_threads,
                                std::size_t total_elements) {
  if (num_threads == 0) {
    throw std::invalid_argument("Number of threads must be greater than 0");
  }

  const auto chunk_size = total_elements / num_threads;
  std::vector<std::uint64_t> partial_sums(num_threads);

  // Launch threads to compute partial sums
  {
    std::vector<std::jthread> threads;
    for (auto i : std::views::iota(0uz, num_threads)) {
      threads.emplace_back(compute_partial_sum, num_threads, i, chunk_size,
                           total_elements, std::ref(partial_sums));
    }
  } // jthreads auto-join here

  // Return the total sum using std::reduce
  return std::reduce(std::execution::seq, partial_sums.begin(),
                     partial_sums.end(), std::uint64_t{0});
}

TEST(ComputeTotalSumTest, SingleThread) {
  constexpr std::size_t num_threads = 1;
  constexpr std::size_t total_elements = 10;

  // Expected sum: 0 + 1 + 2 + ... + 9 = 45
  EXPECT_EQ(compute_total_sum(num_threads, total_elements), 45);
}

TEST(ComputeTotalSumTest, MultipleThreadsEvenSplit) {
  constexpr std::size_t num_threads = 2;
  constexpr std::size_t total_elements = 10;

  // Expected sum: 0 + 1 + 2 + ... + 9 = 45
  EXPECT_EQ(compute_total_sum(num_threads, total_elements), 45);
}

TEST(ComputeTotalSumTest, MultipleThreadsUnevenSplit) {
  constexpr std::size_t num_threads = 3;
  constexpr std::size_t total_elements = 10;

  // Expected sum: 0 + 1 + 2 + ... + 9 = 45
  EXPECT_EQ(compute_total_sum(num_threads, total_elements), 45);
}

TEST(ComputeTotalSumTest, LargeNumberOfElements) {
  constexpr std::size_t num_threads = 4;
  constexpr std::size_t total_elements = 1'000'000;

  // Expected sum: Sum of integers from 0 to 999,999
  constexpr std::uint64_t expected_sum =
      (total_elements * (total_elements - 1)) / 2;
  EXPECT_EQ(compute_total_sum(num_threads, total_elements), expected_sum);
}

TEST(ComputeTotalSumTest, ZeroElements) {
  constexpr std::size_t num_threads = 4;
  constexpr std::size_t total_elements = 0;

  // Expected sum: 0
  EXPECT_EQ(compute_total_sum(num_threads, total_elements), 0);
}

TEST(ComputeTotalSumTest, SingleElement) {
  constexpr std::size_t num_threads = 4;
  constexpr std::size_t total_elements = 1;

  // Expected sum: 0
  EXPECT_EQ(compute_total_sum(num_threads, total_elements), 0);
}

TEST(ComputeTotalSumTest, MoreThreadsThanElements) {
  constexpr std::size_t num_threads = 20;
  constexpr std::size_t total_elements = 10;
  EXPECT_EQ(compute_total_sum(num_threads, total_elements), 45);
}

TEST(ComputeTotalSumTest, ThreadsEqualToElements) {
  constexpr std::size_t num_threads = 10;
  constexpr std::size_t total_elements = 10;
  EXPECT_EQ(compute_total_sum(num_threads, total_elements), 45);
}

TEST(ComputeTotalSumTest, ThousandsOfThreads) {
  constexpr std::size_t num_threads = 2000;
  constexpr std::size_t total_elements = 2000;

  // Sum of [0..1999]
  constexpr std::uint64_t expected_sum =
      (total_elements * (total_elements - 1)) / 2;

  EXPECT_EQ(compute_total_sum(num_threads, total_elements), expected_sum);
}

TEST(ComputeTotalSumTest, MillionsOfElements) {
  constexpr std::size_t num_threads = 10;
  constexpr std::size_t total_elements = 2'000'000;

  // Sum of [0..1999]
  constexpr std::uint64_t expected_sum =
      (total_elements * (total_elements - 1)) / 2;

  EXPECT_EQ(compute_total_sum(num_threads, total_elements), expected_sum);
}

TEST(ComputeTotalSumTest, HardwareConcurrencyThreads) {
  const auto num_threads = std::max(1u, std::thread::hardware_concurrency());
  constexpr std::size_t total_elements = 1'000'000;

  const auto expected_sum = (total_elements * (total_elements - 1)) / 2;
  EXPECT_EQ(compute_total_sum(num_threads, total_elements), expected_sum);
}

TEST(ComputeTotalSumTest, ZeroThreadsShouldFail) {
  EXPECT_THROW(compute_total_sum(0, 10), std::invalid_argument);
}