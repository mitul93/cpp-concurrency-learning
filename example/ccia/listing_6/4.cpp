#include <iostream>
#include <memory>

template <typename T>
class queue_example {
  private:
    struct node {
        T data;
        std::unique_ptr<node> next;

        node(T data_) : data(std::move(data_)) {
        }
    };

    std::unique_ptr<node> head;
    node* tail = nullptr;

  public:
    queue_example() {
    }

    queue_example(const queue_example& other) = delete;
    queue_example& operator=(const queue_example& other) = delete;

    std::shared_ptr<T> try_pop() {
        if (!head) {
            return std::shared_ptr<T>();
        }

        const auto res = std::make_shared<T>(std::move(head->data));
        const auto old_head = std::move(head);
        head = std::move(old_head->next);
        return res;
    }

    void push(T new_value) {
        auto p = std::make_unique<node>(std::move(new_value));
        node* const new_tail = p.get();
        if (tail) {
            tail->next = std::move(p);
        } else {
            head = std::move(p);
        }
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