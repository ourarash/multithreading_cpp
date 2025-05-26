# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary

cc_library(
    name = "map_reduce",
    hdrs = [
        "map_reduce.h",
        "thread_safe_map.h",
    ],
    copts = [
        "-std=c++23",
    ],
    includes = ["."],  # optional: makes local includes work
    visibility = ["//visibility:public"],
)
