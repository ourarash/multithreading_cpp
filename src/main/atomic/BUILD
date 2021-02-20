load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "vector_accumulator_benchmark",
    srcs = ["vector_accumulator_benchmark.cc"],
    deps = [
        "//src/lib:VectorAccumulatorLib",
        "@com_google_benchmark//:benchmark",
    ],
)

cc_binary(
    name = "range_accumulator_benchmark",
    srcs = ["range_accumulator_benchmark.cc"],
    deps = [
        "//src/lib:RangeAccumulatorLib",
        "@com_google_benchmark//:benchmark",
    ],
)

cc_binary(
    name = "atomic_demo_main",
    srcs = ["atomic_demo_main.cc"],
    deps = [
        "@com_google_benchmark//:benchmark",
    ],
)

cc_binary(
    name = "compare_exchange_main",
    srcs = ["compare_exchange_main.cc"],
    deps = [
        "@com_google_benchmark//:benchmark",
    ],
)

cc_binary(
    name = "lock_free_main",
    srcs = ["lock_free_main.cc"],
    deps = [
        "@com_google_benchmark//:benchmark",
    ],
)

cc_binary(
    name = "fetch_main",
    srcs = ["fetch_main.cc"],
    deps = [
        "@com_google_benchmark//:benchmark",
    ],
)


cc_binary(
    name = "sequential_consistency_main",
    srcs = ["sequential_consistency_main.cc"],
    deps = [
        "@com_google_benchmark//:benchmark",
    ],
)
