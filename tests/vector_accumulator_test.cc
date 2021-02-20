#include "src/lib/vector_accumulator.h"

#include <numeric>
#include <random>
#include <vector>

#include "gtest/gtest.h"

std::mutex VectorAccumulator::_my_mutex;
unsigned long VectorAccumulator::_mutex_sum(0);
std::atomic<unsigned long> VectorAccumulator::_atomic_sum(0);

void FillWithRandomValues(std::vector<unsigned long>& a) {
  // First create an instance of an engine.
  std::random_device rnd_device;
  // Specify the engine and distribution.
  std::mt19937 mersenne_engine{rnd_device()};  // Generates random integers
  std::uniform_int_distribution<int> dist{0, 1};

  auto gen = [&dist, &mersenne_engine]() { return dist(mersenne_engine); };

  generate(std::begin(a), std::end(a), gen);
}

const unsigned long g_size = 100000000;

template <class FT, class RT>
void GenericTest(FT func, RT& result) {
  std::vector<unsigned long> in;
  unsigned long expected;

  // Check empty
  in = {};
  expected = 0;

  for (int number_of_threads = 1; number_of_threads < 12; number_of_threads++) {
    VectorAccumulator::Driver(in, number_of_threads, func);
    EXPECT_EQ(expected, result);
  }

  // Check random
  in.resize(g_size);
  FillWithRandomValues(in);
  expected = std::accumulate(in.begin(), in.end(), 0);

  for (int number_of_threads = 1; number_of_threads < 5; number_of_threads++) {
    VectorAccumulator::Driver(in, number_of_threads, func);
    EXPECT_EQ(expected, result);
  }
}

TEST(VectorAccumulatorTest, AtomicAccumulator) {
  GenericTest(VectorAccumulator::AtomicAccumulator, VectorAccumulator::_atomic_sum);
}

TEST(VectorAccumulatorTest, AtomicAccumulatorRelaxed) {
  GenericTest(VectorAccumulator::AtomicAccumulatorRelaxed, VectorAccumulator::_atomic_sum);
}

TEST(VectorAccumulatorTest, AtomicAccumulatorPartitionRelaxed) {
  GenericTest(VectorAccumulator::AtomicAccumulatorPartitionRelaxed,
           VectorAccumulator::_atomic_sum);
}

TEST(VectorAccumulatorTest, MutexAccumulatorPartition) {
  GenericTest(VectorAccumulator::MutexAccumulatorPartition, VectorAccumulator::_mutex_sum);
}
