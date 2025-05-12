# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_test(
    name = "jthread",
    srcs = ["jthread.cc"],
    copts = [
        "-std=c++23",
    ],
    deps = [
        "//src/lib:utility_lib",
        "@googletest//:gtest_main",
    ],
)

cc_binary(
    name = "lambda_jthread_main",
    srcs = ["lambda_jthread_main.cc"],
    # copts = [
    #     "-std=c++20",
    #     "-stdlib=libc++",
    # ],
    deps = ["//src/lib:utility_lib"],
)
