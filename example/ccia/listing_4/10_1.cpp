/**
 * Example not in the book
 * It simulates scenario to show that future.get() blocks the thread until
 * future sets the value
 */

#include <chrono>
#include <future>
#include <iostream>
#include <thread>

int main() {

    std::promise<std::string> promise;

    std::future<std::string> reply = promise.get_future();

    std::thread worker([&promise] {
        std::cout << "Worker: processing...\n";

        std::this_thread::sleep_for(std::chrono::seconds(2));

        std::cout << "Worker: setting promise\n";
        promise.set_value("Hello client");
    });

    std::cout << "Main: waiting for reply...\n";

    // Blocks for approximately 2 seconds
    std::string result = reply.get();

    std::cout << "Main: got reply = " << result << '\n';

    worker.join();
}