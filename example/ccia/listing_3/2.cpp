#include <iostream>
#include <mutex>
#include <string>

class some_data {
    int a;
    std::string b;

  public:
    void do_something() {
        std::cout << "doing something\n";
    }
};

class data_wrapper {
  private:
    std::mutex m;
    some_data data;

  public:
    template <typename Function>
    void process_data(Function func) {
        // std::lock_guard<std::mutex> l(m); // Also valid
        std::lock_guard l(m);
        func(data);
    }
};

some_data* unprotected;

void malicious_function(some_data& protected_data) {
    unprotected = &protected_data;
}

data_wrapper x;

void foo() {
    x.process_data(malicious_function);
    // call to do_something without mutex
    unprotected->do_something();
}

int main() {
    foo();
    return 1;
}