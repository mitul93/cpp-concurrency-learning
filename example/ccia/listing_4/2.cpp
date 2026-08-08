/*
 * This is not a complete, standalone example.
 * The code is intended only to illustrate the concept.
 * This code will compile, but it will do noting!
 */

#include <deque>

template <class T, class Container = std::deque<T>>
class queue {
  public:
    explicit queue(const Container&);
    explicit queue(Container&& = Container());

    template <class Alloc>
    explicit queue(const Alloc&);

    template <class Alloc>
    queue(const Container&, const Alloc&);

    template <class Alloc>
    queue(Container&&, const Alloc&);

    template <class Alloc>
    queue(queue&&, const Alloc&);

    void swap(queue& q);

    bool empty() const;
    size_t size() const;

    T& front();
    const T& front() const;

    T& back();
    const T& back() const;

    void push(const T& x);
    void push(T&& x);

    void pop();

    template <class... Args>
    void emplace(Args&&... args);
};

int main() {

    return 0;
}
