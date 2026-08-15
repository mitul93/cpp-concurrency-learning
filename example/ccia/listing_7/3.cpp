#include <atomic>
#include <memory>
#include <ranges>

template <typename T>
class lock_free_stack {
  private:
    struct node {
        std::shared_ptr<T> data;
        node* next;

        node(const T& data_) : data(std::make_shared<T>(data_)) {
        }
    };

    std::atomic<node*> head;

  public:
    void push(const T& data) {
        node* const new_node = new node(data);
        new_node->next = head.load();

        while (!head.compare_exchange_weak(new_node->next, new_node)) {
        };
    }

    std::shared_ptr<T> pop() {
        node* old_head = head.load();
        while (old_head &&
               !head.compare_exchange_weak(old_head, old_head->next)) {
        }

        // If CAS is successful, head points to old_head->next.
        // This means that delete on old_head will never be called
        // So this program leaks node every time pop() is called

        // Q. Why you cannot delete old_head here?
        // the problem is other thread may still be dereferencing `old_head`
        // after old_head is delted here. Another thread might be calling
        // `pop()` concurrently may have read value of `old_head` pointer.
        // So memory leak in pop() is deleberate in this code.

        // delete old_head;

        // Q. Why old_head can be nullptr here?
        // even when head.load() is not nullptr, head can be nullptr in CAS.
        return old_head ? old_head->data : std::shared_ptr<T>();

        // Not OK : Initializer list cannot be used on the right hand side of
        // operator ':'clang(init_list_bin_op)
        // return old_head ? old_head->data : {};
    }
};

int main() {
    lock_free_stack<int> stack;

    int num_operations = 100;

    for (int index : std::views::iota(0, num_operations)) {
        stack.push(index);
    }

    for (int index : std::views::iota(0, num_operations)) {
        // Note here that if pop() is not called by program,
        // Clang leak sanitizer (-fsanitize=leak) cannot detect node leak in
        // pop() operation
        auto val = stack.pop();
    }

    return 0;
}