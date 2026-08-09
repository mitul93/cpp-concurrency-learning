#include <condition_variable>
#include <memory>
#include <mutex>

template <typename T>
class threadsafe_queue {
  private:
    struct node {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };

    std::mutex head_mutex;
    std::mutex tail_mutex;
    std::condition_variable data_cond;

    std::unique_ptr<node> head;
    node* tail;

    node* get_tail() {
        std::lock_guard tail_lock(tail_mutex); // CTAD
        return tail;
    }

    std::unique_ptr<node> pop_head() {
        auto old_head = std::move(head);
        head = std::move(old_head->next);
        return old_head;
    }

    std::unique_ptr<T> try_pop_head() {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if (head.get() == get_tail()) {
            return std::unique_ptr<node>();
        }

        return pop_head();
    }

    std::unique_ptr<node> try_pop_head(T& value) {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if (head.get() == get_tail()) {
            return std::unique_ptr<node>();
        }
        value = std::move(*head->data);
        return pop_head();
    }

  public:
    std::shared_ptr<T> try_pop() {
        auto old_head = try_pop_head();
        return old_head ? old_head->data : std::shared_ptr<T>();
    }

    bool try_pop(T& value) {
        const auto old_head = try_pop_head(value);
        return old_head;
    }

    void empty() {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        return (head.get() == get_tail());
    }
};

int main() {
    return 0;
}