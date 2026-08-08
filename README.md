# cpp-concurrency-learning

C++ development container for VS Code that supports both Docker and Podman.

## Purpose

This repository is primarily a **personal educational and learning purposes**. I use it to study and deepen my understanding of C++ concurrency concepts.

I am using **[C++ Concurrency in Action](https://www.manning.com/books/c-plus-plus-concurrency-in-action)** by Anthony Williams as my main reference, but the material in this repository is **not limited to that book**. I may also explore concepts from other resources, documentation, and my own experiments.

I do **not** claim ownership of the code originating from *C++ Concurrency in Action*. The original code and concepts belong to their respective author and publisher.

The examples may have been modified, extended, or accompanied by additional experiments to help me understand the material.

The code is written as part of my own learning process. Wherever possible, I also create small tests and experiments to explore the examples and better understand how the concepts behave in practice.


## Features

- Works with VS Code Dev Containers
- Supports Docker and Podman
- Non-root user inside container
- UID/GID matching with host
- Forwards host SSH keys for Git access

---

## Requirements

- VS Code
- Dev Containers extension
- Docker or Podman installed on Host

---

## Prerequisite
### Using with Podman
#### Install and verify podman
Make sure that podman is installed and accessible
```bash
$ podman --version
```
#### Configure VS Code to use Podman
Open VS Code settings for workspace
- `Ctrl + Shift + P` -> Preference Open Workspace Settings
- Search `containers.containerClient` and set it to `podman`. This tells the VS Code Dev Containers extension to use Podman instead of Docker.

### Using with Docker
TODO : Test and Write

## Open project in VS Code
Clone this repository (if you haven’t already) and open it in VS Code.

## Start Devcontainer
Press Ctrl + Shift + P → Dev Containers: Reopen in Container

## Build hello world inside devcontainer
```
mkdir -p build
cd build
cmake ..
cmake --build .
```

```
$ cd build
$ ./hello_world/hello_world 
Hello and welcome to C++!
i = 1
i = 2
i = 3
i = 4
i = 5
```

# Setup Troubleshooting

```
/usr/bin/ld: cannot find /usr/lib/llvm-19/lib/clang/19/lib/x86_64-pc-linux-gnu/libclang_rt.tsan.a: No such file or directory
/usr/bin/ld: cannot find /usr/lib/llvm-19/lib/clang/19/lib/x86_64-pc-linux-gnu/libclang_rt.tsan_cxx.a: No such file or directory
```

```
apt install libclang-rt-dev
```

##
```
ThreadSanitizer: CHECK failed: tsan_platform_linux.cpp:290 "((personality(old_personality | ADDR_NO_RANDOMIZE))) != ((-1))" (0xffffffffffffffff, 0xffffffffffffffff) (tid=16446)
Segmentation fault (core dumped)
```
set devcontainer secomp permission

## TSan not showing symbols
```
WARNING: ThreadSanitizer: data race (pid=1943)
  Write of size 4 at 0x555556a6bb28 by thread T2:
    #0 <null> <null> (data_race_example+0xe9858) (BuildId: 95f1561dc6af672c907aaca14422cba18475c727)
    #1 <null> <null> (data_race_example+0xea192) (BuildId: 95f1561dc6af672c907aaca14422cba18475c727)
    #2 <null> <null> (data_race_example+0xea0f5) (BuildId: 95f1561dc6af672c907aaca14422cba18475c727)
    #3 <null> <null> (data_race_example+0xea0ad) (BuildId: 95f1561dc6af672c907aaca14422cba18475c727)
    #4 <null> <null> (data_race_example+0xea055) (BuildId: 95f1561dc6af672c907aaca14422cba18475c727)
    #5 <null> <null> (data_race_example+0xe9ec9) (BuildId: 95f1561dc6af672c907aaca14422cba18475c727)
    #6 <null> <null> (libstdc++.so.6+0xe1223) (BuildId: 133b71e0013695cc7832680a74edb51008c4fc4c)
```
install `llvm`

## Why sanitizer symbol shows up with /usr/lib/gcc

```
    #0 increment() /cpp-concurrency-learning/example/data_race/example.cpp:8:9 (data_race_example+0xe9858) (BuildId: 95f1561dc6af672c907aaca14422cba18475c727)
    #1 void std::__invoke_impl<void, void (*)()>(std::__invoke_other, void (*&&)()) /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/invoke.h:61:14 (data_race_example+0xea192) (BuildId: 95f1561dc6af672c907aaca14422cba18475c727)
    #2 std::__invoke_result<void (*)()>::type std::__invoke<void (*)()>(void (*&&)()) /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/invoke.h:96:14 (data_race_example+0xea0f5) (BuildId: 95f1561dc6af672c907aaca14422cba18475c727)
    #3 void std::thread::_Invoker<std::tuple<void (*)()>>::_M_invoke<0ul>(std::_Index_tuple<0ul>) /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/std_thread.h:301:13 (data_race_example+0xea0ad) (BuildId: 95f1561dc6af672c907aaca14422cba18475c727)
    #4 std::thread::_Invoker<std::tuple<void (*)()>>::operator()() /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/std_thread.h:308:11 (data_race_example+0xea055) (BuildId: 95f1561dc6af672c907aaca14422cba18475c727)
    #5 std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (*)()>>>::_M_run() /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/std_thread.h:253:13 (data_race_example+0xe9ec9) (BuildId: 95f1561dc6af672c907aaca14422cba18475c727)
    #6 <null> <null> (libstdc++.so.6+0xe1223) (BuildId: 133b71e0013695cc7832680a74edb51008c4fc4c)
```
This is normal. These are just libc++ headers. The compiler is still clang++.