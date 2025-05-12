# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = ["//src/lib:utility_lib"],
)

cc_binary(
    name = "jthread",
    srcs = ["jthread.cc"],
    copts = [
        "-std=c++20",
        "-stdlib=libc++",
    ],
    deps = ["//src/lib:utility_lib"],
)

cc_binary(
    name = "vector_of_threads_main",
    srcs = ["vector_of_threads_main.cc"],
    deps = ["//src/lib:utility_lib"],
)

cc_binary(
    name = "lambda_main",
    srcs = ["lambda_main.cc"],
    deps = ["//src/lib:utility_lib"],
)

cc_binary(
    name = "lambda_jthread_main",
    srcs = ["lambda_jthread_main.cc"],
    copts = [
        "-std=c++20",
        "-stdlib=libc++",
    ],
    deps = ["//src/lib:utility_lib"],
)

cc_binary(
    name = "functors_main",
    srcs = ["functors_main.cc"],
    deps = ["//src/lib:utility_lib"],
)

cc_binary(
    name = "functors_main_unique_ptr",
    srcs = ["functors_main_unique_ptr.cc"],
    deps = ["//src/lib:utility_lib"],
)

cc_binary(
    name = "async_main",
    srcs = ["async_main.cc"],
    deps = ["//src/lib:utility_lib"],
)

cc_binary(
    name = "async_lambda_main",
    srcs = ["async_lambda_main.cc"],
    deps = ["//src/lib:utility_lib"],
)

cc_binary(
    name = "number_of_cores",
    srcs = ["number_of_cores.cc"],
    deps = ["//src/lib:utility_lib"],
)
