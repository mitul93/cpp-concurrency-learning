#include <algorithm> // for_each
#include <atomic>
#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <ranges>
#include <stack>
#include <thread>
#include <vector>

struct empty_stack : std::exception {
    const char* what() const throw() {
        return "empty stack";
    }
};

template <typename T>
class threadsafe_stack {
  private:
    std::stack<T> data;
    mutable std::mutex m;

  public:
    threadsafe_stack() {
    }

    threadsafe_stack(const threadsafe_stack& other) {
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    }

    // Note that only copy assignment need to be deleted
    // move constructor and move assignment is not declared (Hinnat Table)
    threadsafe_stack& operator=(const threadsafe_stack&) = delete;

    void push(T new_value) {
        std::lock_guard<std::mutex> lock(m);
        // might throw an exception if copy/move throws exception, but
        // std::stack guarantees that it won't throw exception!
        data.push(std::move(new_value));
    }

    std::shared_ptr<T> pop() {
        std::lock_guard<std::mutex> lock(m);

        if (data.empty()) {
            throw empty_stack();
        }

        // might throw an exception
        // std::shared_ptr can throw because it cannot allocate memory
        // data can throw because move/copy throws
        const std::shared_ptr<T> res(
            std::make_shared<T>(std::move(data.top())));

        // do not throw
        data.pop();
        return res;
    }

    void pop(T& value) {
        std::lock_guard<std::mutex> lock(m);

        if (data.empty()) {
            throw empty_stack();
        }

        // might throw an exception
        // data can throw because move/copy throws
        value = std::move(data.top());
        data.pop();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};

int main() {
    threadsafe_stack<int> stack;

    // higher number of producers and consumers, higher contention on mutex
    // increasing the producers and consumers, program will slow down eventually
    constexpr int num_producers = 4;
    constexpr int num_consumers = 2;
    constexpr int items_per_producer = 100'000;

    constexpr int total_items = num_producers * items_per_producer;

    // producers
    std::vector<std::thread> producers;
    std::atomic<int> produced{0};

    for (int p : std::views::iota(0, num_producers)) {
        producers.emplace_back([&, p] {
            for (int i : std::views::iota(0, items_per_producer)) {
                int value = p * items_per_producer + i;
                stack.push(value);
                ++produced;
            }
        });
        std::cout << "started producer " << p << "\n";
    }

    // consumers
    std::vector<std::thread> consumers;
    std::atomic<int> consumed{0};

    for (int c : std::views::iota(0, num_consumers)) {
        consumers.emplace_back([&] {
            while (consumed < total_items) {
                try {
                    int value;
                    stack.pop(value);
                    ++consumed;
                } catch (const empty_stack&) {
                    // Stack is temporarily empty.
                    // Try again.
                    std::this_thread::yield();
                }
            }
        });
        std::cout << "started consumer " << c << "\n";
    }

    // Wait for producers
    std::ranges::for_each(producers, &std::thread::join);

    // Wait for consumers
    std::ranges::for_each(consumers, &std::thread::join);

    std::cout << "Produced: " << produced << '\n';
    std::cout << "Consumed: " << consumed << '\n';
    std::cout << "Stack empty: " << std::boolalpha << stack.empty() << '\n';

    if (produced == total_items && consumed == total_items && stack.empty()) {
        std::cout << "SUCCESS: stack behaved correctly.\n";
    } else {
        std::cout << "FAILURE!\n";
    }

    return 0;
}
