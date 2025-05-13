/**
 * A demo for creating jthreads.
 * The code calculates the sum of integers from 0 to total_elements - 1 using
 * multiple threads. Each thread computes a partial sum of a range of numbers,
 * and the results are combined to get the total sum.
 *
 * Note that this code uses C++23 features and it contains the GTEST unit tests.
 * Run this using one of the following methods:
 * 1. With bazel: bazel test //src/main/jthread
 * 2. With plain g++: g++ -std=c++23 -lpthread
 * /Users/ari/github/multithreading_cpp/src/main/jthread/{THIS_FILE_NAME}.cc -I
 * ./
 */
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
void compute_partial_sum(std::size_t num_threads, std::size_t thread_index,
                         std::size_t total_elements,
                         std::vector<std::uint64_t> &partial_sums) {
  // Calculate the size of each chunk
  const auto chunk_size = total_elements / num_threads;

  // Determine the start and end indices for this thread's range
  const auto start_index = thread_index * chunk_size;
  const auto end_index =
      (thread_index == num_threads - 1)
          ? total_elements // Last thread handles the remainder
          : start_index + chunk_size;

  // Generate the range of numbers for this thread
  auto range = std::views::iota(start_index, end_index);

  // Compute the sum of the range and store it in the partial_sums vector
  partial_sums[thread_index] =
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

  std::vector<std::uint64_t> partial_sums(num_threads);

  // Launch threads to compute partial sums
  {
    std::vector<std::jthread> threads;
    for (auto i : std::views::iota(0uz, num_threads)) {
      threads.emplace_back(compute_partial_sum, num_threads, i, total_elements,
                           std::ref(partial_sums));
    }
  } // jthreads auto-join here

  // Return the total sum using std::reduce
  return std::reduce(std::execution::seq, partial_sums.begin(),
                     partial_sums.end(), std::uint64_t{0});
}

//-----------------------------------------------------------------------------
/**
 * Unit tests for the compute_total_sum function.
 */

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