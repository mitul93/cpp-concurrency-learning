#include <future>
#include <iostream>
#include <string>
#include <thread>

struct X {
    void foo(int, const std::string& str) {
        std::cout << "X->foo(): &X=" << this << " str=" << str << std::endl;
    };

    std::string bar(const std::string& str) {
        std::cout << "X->bar(): &X=" << this << " str=" << str << std::endl;
        return str + "_modified";
    }
};

struct Y {
    double operator()(const double x) {
        std::cout << "Y->operator(): &Y=" << this << " x=" << x << std::endl;
        return x * 2.0;
    }
};

X baz(X& x) {
    X new_x;
    return new_x;
}

class move_only {
  public:
    move_only() {
        std::cout << "move_only(): &this=" << this << std::endl;
    }
    move_only(move_only&&) {
        std::cout << "move_only(move_only&&): &this=" << this << std::endl;
    };
    move_only& operator=(move_only&& other) {
        std::cout << "move_only->operator=: &this=" << this << std::endl;
        return *this;
    }

    move_only(const move_only&) = delete;
    move_only& operator=(const move_only&) = delete;

    int operator()() {
        std::cout << "move_only->operator(): this=" << this << std::endl;
        return 2;
    }
};

int main() {

    X x;
    std::cout << "Address of x=" << &x << std::endl;

    auto f1 = std::async(&X::foo, &x, 42, "hello");
    // sleep so that stdout is pretty
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    auto f2 = std::async(&X::bar, x, "goodbye");
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // std::cout << "f1->get()=" << f1.get() << std::endl; // ERROR. f1 is of
    // type std::future<void>
    std::cout << "f2->get()=" << f2.get() << std::endl;
    std::cout << std::endl;

    Y y;
    std::cout << "Address of y=" << &y << std::endl;

    auto f3 = std::async(Y(), 3.141);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    auto f4 = std::async(std::ref(y), 2.718);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    std::cout << "f3->get()=" << f3.get() << std::endl;
    std::cout << "f4->get()=" << f4.get() << std::endl;

    std::cout << std::endl;

    std::cout << "Address of baz()=" << reinterpret_cast<const void*>(&baz)
              << '\n';
    auto f4_1 = std::async(baz, std::ref(x));
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // auto f4_11 = std::async(baz, x); // ERROR
    //    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    std::cout << std::endl;

    // Does not compile without operator() in class
    auto f5 = std::async(move_only());
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    std::cout << "f5->get()=" << f5.get() << std::endl;

    move_only mo;
    // auto f5_1 = std::async(mo); // ERROR. No copy assignment
    // std::this_thread::sleep_for(std::chrono::milliseconds(200));

    auto f5_2 = std::async(std::ref(mo)); // OK. Move assignment
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    std::cout << std::endl;

    // starts a new thread
    auto f6 = std::async(std::launch::async, Y(), 1.2);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "f6->get()=" << f6.get() << std::endl;

    // Run in wait() or get()
    auto f7 = std::async(std::launch::deferred, Y(), 9.0);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    // It will print f7->get() first because Y() is called when get() is called
    std::cout << "f7->get()=" << f7.get() << std::endl;

    // Implemenatation dependent
    auto f8 = std::async(std::launch::async | std::launch::deferred, Y(), 2.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "f8->get()=" << f8.get() << std::endl;

    // Implemenatation dependent
    auto f9 = std::async(Y(), 5.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "f9->get()=" << f9.get() << std::endl;

    // If you do not call f10.get(), the function will never be called
    auto f10 = std::async(std::launch::deferred, Y(), 9.0);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    return 0;
}