load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "tests",
    srcs = glob(["**/*.cc"]),
    deps = [
        "//src/lib:SolutionLib",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "vector_accumulator_test",
    srcs = ["vector_accumulator_test.cc"],
    deps = [
        "//src/lib:VectorAccumulatorLib",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "range_accumulator_test",
    srcs = ["range_accumulator_test.cc"],
    deps = [
        "//src/lib:RangeAccumulatorLib",
        "@googletest//:gtest_main",
    ],
)

