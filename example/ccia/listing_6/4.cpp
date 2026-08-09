#include <iostream>
#include <memory>

template <typename T>
class queue {
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
    queue() {
    }

    queue(const queue& other) = delete;
    queue& operator=(const queue& other) = delete;

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
    queue<int> q;

    q.push(1);
    q.push(2);
    q.push(3);

    std::cout << "q.pop()=" << *q.try_pop() << "\n";
    std::cout << "q.pop()=" << *q.try_pop() << "\n";
    std::cout << "q.pop()=" << *q.try_pop() << "\n";

    return 0;
}