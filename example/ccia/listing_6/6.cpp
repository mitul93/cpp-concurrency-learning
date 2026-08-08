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

    std::unique_ptr<node> head;
    node* tail;

    node* get_tail() {
        std::lock_guard tail_lock(tail_mutex); // CTAD
        return tail;
    }

    std::unique_ptr<node> pop_head() {
        std::lock_guard tail_lock(head_mutex); // CTAD
        if (head.get() == tail) {
            return nullptr;
        }

        auto old_head = std::move(head);
        head = std::move(old_head->next);
        return old_head;
    }

  public:
    threadsafe_queue() : head(new node), tail(head.get()) {
    }

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