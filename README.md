# C++ Template for Bazel and Google Test

> Multi Threading Tutorial in C++

Features:

- [x] Creating task and threads
- [x] Using function pointers, functors, and lambda functions
- [x] Futures, promises, and async tasks
- [x] Supports Google [Bazel](https://bazel.build/)


## Installation

```bash
git clone https://github.com/ourarash/multithreading_cpp.git
```

## Running examples:

You can run this either using `bazel` OR just plain `g++`.


For `bazel`:

```bash
bazel run src/main:main
```

For `g++`:
```bash
g++ -std=c++17 -lpthread src/main/main.cc  -I ./
./a.out
```

## Bazel Installation and Visual Studio Setup
For instructions on installing Bazel and setting it up in Visual Studio Code see [here](https://github.com/ourarash/cpp-template)