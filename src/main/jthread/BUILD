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
    name = "good_jthread_stop_with_callback",
    srcs = ["good_jthread_stop_with_callback.cc"],
    copts = [
        "-std=c++23",
    ],
    deps = [
        "//src/lib:utility_lib",
        # "@googletest//:gtest_main",
    ],
)

cc_binary(
    name = "better_jthread_stop_with_callback",
    srcs = ["better_jthread_stop_with_callback.cc"],
    copts = [
        "-std=c++23",
    ],
    deps = [
        "//src/lib:utility_lib",
        # "@googletest//:gtest_main",
    ],
)

cc_binary(
    name = "better_jthread_stop_with_callback2",
    srcs = ["better_jthread_stop_with_callback2.cc"],
    copts = [
        "-std=c++23",
    ],
    deps = [
        "//src/lib:utility_lib",
        # "@googletest//:gtest_main",
    ],
)

cc_binary(
    name = "bad_thread_stop_1",
    srcs = ["bad_thread_stop_2.cc"],
    copts = [
        "-std=c++23",
    ],
    deps = [
        "//src/lib:utility_lib",
        # "@googletest//:gtest_main",
    ],
)

cc_binary(
    name = "bad_thread_stop_2",
    srcs = ["bad_thread_stop_2.cc"],
    copts = [
        "-std=c++23",
    ],
    deps = [
        "//src/lib:utility_lib",
        # "@googletest//:gtest_main",
    ],
)

cc_binary(
    name = "bad_thread_stop_with_callback",
    srcs = ["bad_thread_stop_with_callback.cc"],
    copts = [
        "-std=c++23",
    ],
    deps = [
        "//src/lib:utility_lib",
        # "@googletest//:gtest_main",
    ],
)

cc_binary(
    name = "good_jthread_stop_1",
    srcs = ["good_jthread_stop_1.cc"],
    copts = [
        "-std=c++23",
    ],
    deps = [
        "//src/lib:utility_lib",
        # "@googletest//:gtest_main",
    ],
)

cc_binary(
    name = "good_jthread_stop_2",
    srcs = ["good_jthread_stop_2.cc"],
    copts = [
        "-std=c++23",
    ],
    deps = [
        "//src/lib:utility_lib",
        # "@googletest//:gtest_main",
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
