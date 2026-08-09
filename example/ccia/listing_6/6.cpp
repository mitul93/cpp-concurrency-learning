#include <iostream>
#include <memory>

template <typename T>
class threadsafe_queue {
  private:
    struct node {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };

    std::mutex head_mutex;
    std::mutex tail_mutex;

    // when threadsafe_queue object is destroyed, unique_ptr destroys head
    // which in turn destroys head->next (also unique_ptr) and so on
    // This is why tail is a raw pointer, not smart pointer
    std::unique_ptr<node> head;
    node* tail;

    node* get_tail() {
        std::lock_guard tail_lock(tail_mutex); // CTAD
        return tail;
    }

    std::unique_ptr<node> pop_head() {
        std::lock_guard tail_lock(head_mutex); // CTAD
        if (head.get() == get_tail()) {
            return nullptr;
        }

        auto old_head = std::move(head);
        head = std::move(old_head->next);
        return old_head;
    }

  public:
    // std::make_unique<T> since c++14
    threadsafe_queue() : head(std::make_unique<node>()), tail(head.get()) {
    }

    // // Also valid before c++14
    // threadsafe_queue() : head(new node), tail(head.get()) {
    // }

    // side note. order of initialization of members? Is following valid?
    // threadsafe_queue() : tail(head.get()), head(std::make_unique<node>()) {
    // }

    threadsafe_queue(const threadsafe_queue& other) = delete;
    threadsafe_queue& operator=(const threadsafe_queue& other) = delete;

    std::shared_ptr<T> try_pop() {
        auto old_head = pop_head();
        return old_head ? old_head->data : std::shared_ptr<T>();
    }

    void push(T new_value) {
        auto new_data = std::make_shared<T>(std::move(new_value));
        auto p = std::make_unique<node>();

        node* const new_tail = p.get();

        std::lock_guard tail_lock(tail_mutex);
        tail->data = new_data;
        tail->next = std::move(p);
        tail = new_tail;
    }
};

int main() {
    threadsafe_queue<int> q;

    q.push(1);
    q.push(2);
    q.push(3);

    std::cout << "q.pop()=" << *q.try_pop() << "\n";
    std::cout << "q.pop()=" << *q.try_pop() << "\n";
    std::cout << "q.pop()=" << *q.try_pop() << "\n";

    return 0;
}