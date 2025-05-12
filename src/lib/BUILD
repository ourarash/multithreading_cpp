cc_library(
    name = "utility_lib",
    # srcs = glob(["**/*.cc"]),
    hdrs = ["utility.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "VectorAccumulatorLib",
    hdrs = ["vector_accumulator.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "RangeAccumulatorLib",
    hdrs = ["range_accumulator.h"],
    visibility = ["//visibility:public"],
    deps = [
        "@com_google_benchmark//:benchmark",
    ],
)
