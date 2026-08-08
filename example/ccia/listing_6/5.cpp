#include <iostream>
#include <memory>

template <typename T>
class queue_example {
  private:
    struct node {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };

    std::unique_ptr<node> head;
    node* tail;

  public:
    queue_example() : head(new node), tail(head.get()) {
    }

    queue_example(const queue_example& other) = delete;
    queue_example& operator=(const queue_example& other) = delete;

    std::shared_ptr<T> try_pop() {
        if (head.get() == tail) {
            return std::shared_ptr<T>();
        }

        const auto res = head->data;
        const auto old_head = std::move(head);
        head = std::move(old_head->next);
        return res;
    }

    void push(T new_value) {
        auto new_data = std::make_shared<T>(std::move(new_value));
        auto p = std::make_unique<node>();

        tail->data = new_data;
        node* const new_tail = p.get();
        tail->next = std::move(p);
        tail = new_tail;
    }
};

int main() {
    queue_example<int> q;

    q.push(1);
    q.push(2);
    q.push(3);

    std::cout << "q.pop()=" << *q.try_pop() << "\n";
    std::cout << "q.pop()=" << *q.try_pop() << "\n";
    std::cout << "q.pop()=" << *q.try_pop() << "\n";

    return 0;
}