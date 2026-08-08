// listing_4/3.cpp
#include <algorithm> // for_each
#include <condition_variable>
#include <iostream>
#include <memory>
#include <queue>
#include <ranges>
#include <thread>
#include <vector>

template <typename T>
class threadsafe_queue {
  private:
    mutable std::mutex mut;
    std::queue<std::shared_ptr<T>> data_queue;
    std::condition_variable data_cond;

  public:
    threadsafe_queue() {
    }

    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] { return !data_queue.empty(); });
        value = std::move(*data_queue.front());
        data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] { return !data_queue.empty(); });
        auto res = data_queue.front();
        data_queue.pop();
        return res;
    }

    std::shared_ptr<T> try_pop(T& value) {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty()) {
            return std::shared_ptr<T>();
        }

        auto res = data_queue.front();
        data_queue.pop();
        return res;
    }

    void push(T new_value) {
        auto data = std::make_shared<T>(std::move(new_value));
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data);
        data_cond.notify_one();
    }

    bool empty() const {
        std::lock_guard lk(mut); // CTAD
        return data_queue.empty();
    }
};

int main() {

    threadsafe_queue<int> queue;

    // If number of consumers are more than 1, it is highly likely that
    // consumers thread will not join at the end of the program. Why?
    // wait_and_pop() execution inside consumer thread locks the mutex until the
    // queue is empty.and push() only notifies one thread.
    // Picture a scenario where consumer 0 and consumer 1 both
    // waiting for the last item in the queue notify_one() only notifies
    // consumer 0. consumer 1 is still waiting.
    constexpr int num_producers = 10;
    constexpr int num_consumers = 2;
    constexpr int items_per_producer = 1000;

    constexpr int total_items = num_producers * items_per_producer;

    // producers
    std::vector<std::thread> producers;
    std::atomic<int> produced{0};

    for (int p : std::views::iota(0, num_producers)) {
        producers.emplace_back([&, p] {
            for (int i : std::views::iota(0, items_per_producer)) {
                int value = p * items_per_producer + i;
                queue.push(value);
                // std::cout << "thread " << p << " pushed " << value << "\n";
                ++produced;
            }
        });
        std::cout << "started producer " << p << "\n";
    }

    // consumers
    std::vector<std::thread> consumers;
    std::atomic<int> consumed{0};

    for (int c : std::views::iota(0, num_consumers)) {
        consumers.emplace_back([&, c] {
            while (consumed < total_items) {
                int value;
                queue.wait_and_pop(value);
                ++consumed;
                // std::cout << "thread " << c << " consumed " << value << "\n";
            }
        });
        std::cout << "started consumer " << c << "\n";
    }

    // Wait for producers
    std::ranges::for_each(producers, &std::thread::join);
    std::cout << "joined producer threads\n";

    // Wait for consumers
    // If num_consumers is more than 1, it is likely that code below this line
    // will not execute
    for (int c : std::views::iota(0, num_consumers)) {
        consumers.at(c).join();
        std::cout << "joined consumer " << c << "\n";
    }

    std::cout << "Produced: " << produced << '\n';
    std::cout << "Consumed: " << consumed << '\n';
    std::cout << "Stack empty: " << std::boolalpha << queue.empty() << '\n';

    if (produced == total_items && consumed == total_items && queue.empty()) {
        std::cout << "SUCCESS: stack behaved correctly.\n";
    } else {
        std::cout << "FAILURE!\n";
    }

    return 0;
}