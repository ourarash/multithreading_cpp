# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary

cc_binary(
    name = "count_words",
    srcs = ["count_words.cc"],
    copts = [
        "-std=c++20",
    ],
    deps = ["//src/main/map_reduce/lib:map_reduce"],
)


cc_binary(
    name = "count_words_file",
    srcs = ["count_words_file.cc"],
    copts = [
        "-std=c++20",
    ],
    deps = ["//src/main/map_reduce/lib:map_reduce"],
)
