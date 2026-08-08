#include <chrono>
#include <deque>
#include <future> // for std::packaged_task
#include <iostream>
#include <mutex>
#include <thread>
#include <utility>

std::mutex m;
std::deque<std::packaged_task<void()>> tasks;

std::atomic<bool> shutdown{false};

bool gui_shutdown_message_received() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (shutdown.load()) {
        std::cout << "gui_shutdown_message_received()" << std::endl;
    }

    return shutdown.load();
}

void get_and_process_gui_message() {
    // Simulate GUI message processing
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "get_and_process_gui_message()" << std::endl;
}

void gui_thread() {
    std::cout << "gui_thread() started" << std::endl;

    while (!gui_shutdown_message_received()) {

        get_and_process_gui_message();

        std::packaged_task<void()> task;
        {
            std::lock_guard<std::mutex> lk(m);

            if (tasks.empty()) {
                continue;
            }

            task = std::move(tasks.front());
            tasks.pop_front();
        }
        // Execute task outside lock
        std::cout << "gui_thread(): Executing task" << std::endl;
        // Function is called here
        task();
    }

    std::cout << "gui_thread() stopped" << std::endl;
}

template <typename Func>
std::future<void> post_task_for_gui_thread(Func f) {
    std::packaged_task<void()> task(f);
    std::future<void> res = task.get_future();
    std::lock_guard<std::mutex> lk(m);
    tasks.push_back(std::move(task));
    return res;
}

// This code creates tasks and put them in a queue
// Queue runs one task at a time and moves to the next
// until all tasks are empty or shutdown signal is received
int main() {
    std::cout << "Main thread\n";

    // start GUI thread
    std::thread gui_bg_thread(gui_thread);
    // std::this_thread::sleep_for(std::chrono::seconds(2));

    // Create task 1
    auto f1 = post_task_for_gui_thread([] {
        std::cout << "Task 1 running on GUI thread\n";
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Task 1 completed\n";
    });
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Create task 2
    auto f2 = post_task_for_gui_thread([] {
        std::cout << "Task 2 running on GUI thread\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Task 2 completed\n";
    });

    // Create task 3
    auto f3 = post_task_for_gui_thread([] {
        std::cout << "Task 3 running on GUI thread\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Task 3 completed\n";
    });

    // Simulate delay to show that task execution has started
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // Wait until tasks finish
    std::cout << "f1.get()\n";
    f1.get();

    std::cout << "f2.get()\n";
    f2.get();

    std::cout << "f3.get()\n";
    f3.get();

    std::cout << "All tasks completed\n";

    shutdown = true;

    gui_bg_thread.join();

    return 0;
}