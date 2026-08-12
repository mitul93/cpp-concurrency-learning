#include <algorithm>
#include <atomic>
#include <cassert>
#include <ranges>
#include <thread>
#include <vector>

/*
 * This is not a complete code for lock_free_stack.
 * It only shows implementation of push
 * This code will compile, but it can not verify is push operation
 * does what it is supposed to do!
 */

template <typename T>
class lock_free_stack {
  private:
    struct node {
        T data;
        node* next;

        node(const T& data_) : data(data_) {
        }
    };

    std::atomic<node*> head;

  public:
    void push(const T& data) {

        // Q. Can I use any type as std::atomic?
        assert(head.is_lock_free());

        node* const new_node = new node(data);
        new_node->next = head.load();
        while (!head.compare_exchange_weak(new_node->next, new_node)) {
        }
    }
};

int main() {

    lock_free_stack<int> stack;

    size_t num_workers = 10;
    std::vector<std::thread> workers;
    workers.reserve(num_workers);

    // https://en.cppreference.com/cpp/language/integer_literal
    // since c++23: 0uz --> std::size_t(0)
    // for (size_t index : std::views::iota(0uz, num_workers)) {
    for (size_t index : std::views::iota(std::size_t(0), num_workers)) {
        workers.emplace_back([&stack, &index]() { stack.push(index); });
    }

    std::ranges::for_each(workers, &std::thread::join);

    return 0;
}