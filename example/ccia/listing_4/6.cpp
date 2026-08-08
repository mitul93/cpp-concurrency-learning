#include <chrono>
#include <future>
#include <iostream>
#include <thread>

int find_the_answer_to_ltuae() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "returning find_the_answer_to_ltuae()\n";
    return 1;
}

void do_other_stuff() {
    std::cout << "Sleeping do_ther_stuff() for 5 seconds\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "returning do_other_stuff()\n";
}

int main() {
    auto the_answer = std::async(find_the_answer_to_ltuae);

    do_other_stuff();

    std::cout << "The answer is " << the_answer.get() << "\n";

    return 0;
}