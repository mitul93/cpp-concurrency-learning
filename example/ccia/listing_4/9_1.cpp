/**
 * Example not in the book
 */

#include <future>
#include <iostream>
#include <string>

void foo() {
    std::cout << "foo()" << std::endl;
}

int boo(const std::string& str) {
    std::cout << "boo() str=" << str << std::endl;
    return 1;
}

int main() {
    {
        std::packaged_task<void()> task(foo); // future<void>
        std::future<void> result = task.get_future();

        task(); // <-- executes foo()

        // If you forget above line, the program will just wait forever
        result.get();
    }

    std::cout << "\n";

    {
        std::packaged_task<int(const std::string&)> task(boo); // future<void>
        std::future<int> result = task.get_future();

        task("hello"); // <-- executes boo()

        int val = result.get();
        std::cout << "val=" << val << std::endl;
    }

    std::cout << "\n";

    {
        std::packaged_task task(boo); // future<void>
        auto result = task.get_future();

        task("world"); // <-- executes boo()

        auto val = result.get();
        std::cout << "val=" << val << std::endl;
    }

    return 0;
}