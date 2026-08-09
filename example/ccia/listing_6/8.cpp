/*
 * This is not a complete, standalone example.
 * The code is intended only to illustrate the concept.
 * This code will compile, but it will do noting!
 */
#include <condition_variable>
#include <memory>

template <typename T>
class threadsafe_queue {
    struct node {};
    std::mutex tail_mutex;
    std::condition_variable data_cond;
    node* tail;
    void push(T new_value);
};

template <typename T>
void threadsafe_queue<T>::push(T new_value) {
    auto new_data = std::make_shared<T>(std::move(new_value));
    auto p = std::make_unique<node>();
    {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        tail->data = new_data;
        node* const new_tail = p.get();
        tail->next = std::move(p);
        tail = new_tail;
    }
    data_cond.notify_one();
}

int main() {
    return 0;
}