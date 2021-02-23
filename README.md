# Multi Threading Tutorial in C++

> Quick demonstration of multi threading in modern C++

Features:

- [x] Creating task and threads
- [x] Using function pointers, functors, and lambda functions
- [x] Futures, promises, and async tasks
- [x] Mutex and Locks
- [x] Conditional Variables
- [x] Supports Google [Bazel](https://bazel.build/)



Here is the video explaining how to use multi-threading in C++:

<table><tr><td>

<a href="https://youtu.be/3aqxaZsvn80">
<img border="5" alt="Debugging C++ in Visual Studio Code using gcc/gdb and Bazel" src="https://github.com/ourarash/multithreading_cpp/blob/master/youtube.png?raw=true" width="400">
</a>
</td></tr></table>

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