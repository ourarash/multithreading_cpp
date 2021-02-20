#include "src/lib/range_accumulator.h"

#include <numeric>
#include <random>
#include <vector>

#include "gtest/gtest.h"

std::mutex RangeAccumulator::_my_mutex;
unsigned long RangeAccumulator::_mutex_sum(0);
std::atomic<unsigned long> RangeAccumulator::_atomic_sum(0);

const unsigned long g_size = 1000000000;

template <class FT, class RT>
void GenericTest(FT func, RT& result) {
  unsigned long in;
  unsigned long expected;

  // Check empty
  in = 0;
  expected = 0;

  for (int number_of_threads = 1; number_of_threads < 12; number_of_threads++) {
    RangeAccumulator::Driver(number_of_threads, func, 0);
    EXPECT_EQ(expected, result);
  }

  in = g_size;
  expected = in * (in - 1) / 2;

  for (int number_of_threads = 1; number_of_threads < 5; number_of_threads++) {
    RangeAccumulator::Driver(number_of_threads, func, in);
    EXPECT_EQ(expected, result);
  }
}

TEST(RangeAccumulatorTest, AtomicAccumulator) {
  GenericTest(RangeAccumulator::AtomicAccumulator,
              RangeAccumulator::_atomic_sum);
}

TEST(RangeAccumulatorTest, AtomicAccumulatorRelaxed) {
  GenericTest(RangeAccumulator::AtomicAccumulatorRelaxed,
              RangeAccumulator::_atomic_sum);
}

TEST(RangeAccumulatorTest, AtomicAccumulatorPartitionRelaxed) {
  GenericTest(RangeAccumulator::AtomicAccumulatorPartitionRelaxed,
              RangeAccumulator::_atomic_sum);
}

TEST(RangeAccumulatorTest, MutexAccumulatorPartition) {
  GenericTest(RangeAccumulator::MutexAccumulatorPartition,
              RangeAccumulator::_mutex_sum);
}
