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
        auto old_head = std::move(head);
        head = std::move(old_head->next);
        return old_head;
    }

    std::unique_lock<std::mutex> wait_for_data() {
        std::unique_lock<std::mutex> head_lock(head_mutex);
        data_cond.wait(head_lock, [&] { return head.get() != get_tail(); });
        return std::move(head_lock); // Would this perform move automatically
                                     // since copy is deleted?
    }

    // wait_for_data return unique_lock to make sure A and B can hold the same
    // lock when data is being modified by different overload
    std::unique_ptr<node> wait_pop_head() {
        std::unique_lock<std::mutex> head_lock(wait_for_data()); // A
        return pop_head();
    }

    std::unique_ptr<node> wait_pop_head(T& value) {
        std::unique_lock<std::mutex> head_lock(wait_for_data()); // B
        value = std::move(*head->data);
        return pop_head();
    }

  public:
    std::shared_ptr<T> wait_and_pop() {
        const auto old_head = wait_pop_head();
        return old_head->data;
    }

    void wait_and_pop(T& value) {
        const auto old_head = wait_pop_head(value);
    }
};

int main() {
    return 0;
}