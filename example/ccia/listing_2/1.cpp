#include <iostream>
#include <thread>

void do_something(int& i) {
    i++;
    std::cout << "i=" << i << "\n";
}

struct func {
    int& i;

    func(int& i_) : i(i_) {
    }

    void operator()() {
        for (unsigned j = 0; j < 10; ++j) {
            do_something(i);
        }
    }
};

void oops() {
    int some_local_state = 0;
    func my_func(some_local_state);

    std::cout << "Starting thread\n";

    std::thread my_thread(my_func);
    my_thread.detach();

    std::cout << "Thread detached\n";
}

int main() {
    oops();
    std::cout << "Exiting application. Thread might still be running.\n";
}