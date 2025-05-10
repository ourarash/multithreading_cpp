load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "googletest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.8.1",
)

git_repository(
    name = "com_google_benchmark",
    remote = "https://github.com/google/benchmark.git",
    tag = "v1.5.1",
)

new_local_repository(
    name = "intel",
    build_file = "third_party/BUILD.intel",
    
    # Mac:
    path = "/opt/intel/tbb/",

    # For Linux or windows, modify the above path to the proper value from here:
    # https://software.intel.com/content/www/us/en/develop/documentation/tbb-tutorial/top/tutorial-developing-applications-using-parallelfor/prepare-the-enviroment.html
)
