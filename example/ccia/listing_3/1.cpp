#include <algorithm>
#include <iostream>
#include <list>
#include <mutex>

std::list<int> some_list;
std::mutex some_mutex;

void add_to_list(int new_value) {
    std::lock_guard<std::mutex> guard(some_mutex);
    // // Also Valid
    // std::lock_guard guard(some_mutex);

    some_list.push_back(new_value);
}

bool list_contains(int value_to_find) {
    // Q. Can you protect underlying data with mutex if other function can
    // return pointer or reference to some_list?
    std::lock_guard<std::mutex> guard(some_mutex);
    return std::find(some_list.begin(), some_list.end(), value_to_find) !=
           some_list.end();
}

int main() {
    add_to_list(10);
    add_to_list(50);

    std::cout << "list_contains(10) = " << std::boolalpha << list_contains(10)
              << "\n";

    std::cout << "list_contains(20) = " << std::boolalpha << list_contains(20)
              << "\n";
    return 0;
}

/*
https://godbolt.org/z/ajr6hvE7z

Q. Chasing 'What is the assembly code for mutex lock on Intel?`
A. It's complicated

Here is assembly of std::mutex::lock()

"std::mutex::lock()":
        push    rbp
        mov     rbp, rsp
        sub     rsp, 32
        mov     QWORD PTR [rbp-24], rdi
        mov     rax, QWORD PTR [rbp-24]
        mov     QWORD PTR [rbp-16], rax
        mov     eax, 1
        test    eax, eax
        setne   al
        test    al, al
        je      .L18
        mov     rax, QWORD PTR [rbp-16]
        mov     rdi, rax
        call    "pthread_mutex_lock"
        jmp     .L19
.L18:
        mov     eax, 0
.L19:
        mov     DWORD PTR [rbp-4], eax
        cmp     DWORD PTR [rbp-4], 0
        je      .L21
        mov     eax, DWORD PTR [rbp-4]
        mov     edi, eax
        call    "std::__throw_system_error(int)"
.L21:
        nop
        leave
        ret

Note that Jump to .L18 is just because the code is compiled with `-O0`
It is library's attempt to avoid pthread_mutex_lock in case of single-threaded
machine. However, this could be optimized with `-O1` and higher.

The interesting part is `call "pthread_mutex_lock"`
Compiler explorer cannot show assembly of this call because it it linked with
external library pthread.

c++11 introduced concurrency primitives, before this there was no concept of
threads in c++. However, this doesn't mean that standard libraries cannot use
pthreads under the hood since c++11. c++ have to run on variety of hardware and
platform including linux, windows, embedded, RTOS, etc. So, the standard do not
dicatate how it is implemented.
*/

// clang-format off
/*
```shell
// on older system, you should see libpthread.so 
devcontainer@49aaf2e16bd2:/cpp-concurrency-learning/build$ ldd example/ccia/listing_3_1 
        linux-vdso.so.1 (0x00007ffdc09e8000)
        libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x0000727dcc10b000)
        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x0000727dcc01b000)
        libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x0000727dcbfee000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x0000727dcbdfa000)
        /lib64/ld-linux-x86-64.so.2 (0x0000727dcc37e000)
        
devcontainer@49aaf2e16bd2:/cpp-concurrency-learning/build$ nm -D /lib/x86_64-linux-gnu/libc.so.6 | grep pthread_mutex_lock 0000000000095e60 T
        __pthread_mutex_lock@GLIBC_2.2.5 0000000000095e60 T
        pthread_mutex_lock@@GLIBC_2.2.5

devcontainer@49aaf2e16bd2:/cpp-concurrency-learning/build$ objdump -d /lib/x86_64-linux-gnu/libc.so.6 --start-address=0x95e60 --stop-address=0x96172
```
*/
// clang-format on
