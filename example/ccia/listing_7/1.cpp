#include <algorithm>
#include <atomic>
#include <iostream>
#include <ranges>
#include <thread>
#include <vector>

class spinlock_mutex {

    // Should I mark this mutable so that methods
    // lock() and unlock() can be const?
    std::atomic_flag flag = ATOMIC_FLAG_INIT;

  public:
    // clang++ 19.1.7 : warning: braces around scalar initializer
    // [-Wbraced-scalar-init]
    //  spinlock_mutex() : flag(ATOMIC_FLAG_INIT) { }
    // spinlock_mutex() : flag{ATOMIC_FLAG_INIT} { }

    void lock() {
        while (flag.test_and_set(std::memory_order_acquire)) {
        };
    }

    void unlock() {
        flag.clear(std::memory_order_release);
    }
};

spinlock_mutex s_mutex;

void foo(size_t index) {
    s_mutex.lock();
    std::cout << "foo() : index=" << index << "\n";
    s_mutex.unlock();
}

int main() {
    size_t num_workers = 10;
    std::vector<std::thread> workers;
    workers.reserve(num_workers);

    // https://en.cppreference.com/cpp/language/integer_literal
    // since c++23: 0uz --> std::size_t(0)
    // for (size_t index : std::views::iota(0uz, num_workers)) {
    for (size_t index : std::views::iota(std::size_t(0), num_workers)) {
        workers.emplace_back(foo, index);
    }

    std::ranges::for_each(workers, &std::thread::join);

    return 0;
}