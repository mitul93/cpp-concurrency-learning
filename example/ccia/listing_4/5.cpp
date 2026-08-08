#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

template <typename T>
class threadsafe_queue {
  private:
    // mutable is required because empty() is const and
    // copy constructor arg is const
    // std::lock_guard change mutex value
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;

  public:
    threadsafe_queue() {};

    threadsafe_queue(const threadsafe_queue& other) {
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue = other.data_queue;
    };

    void push(T new_value) {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }

    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this]() { return !data_queue.empty(); });
        value = data_queue.front();
        data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this]() { return !data_queue.empty(); });
        std::shared_ptr<T> res{std::make_shared<T>(data_queue.front())};
        data_queue.pop();
        return res;
    }

    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty()) {
            return false;
        }

        value = data_queue.front();
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop() {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty()) {
            return std::shared_ptr<T>();
        }

        std::shared_ptr<T> res{std::make_shared<T>(data_queue.front())};
        data_queue.pop();
        return res;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.size();
    }
};

struct data_chunk {
    unsigned int data;
};

const unsigned int max_counter_val = 10;

threadsafe_queue<data_chunk> data_queue;

void process(const data_chunk& chunk) {
    std::cout << "process() : data_chunk data=" << chunk.data << "\n";
}

void data_preparation_thread() {
    unsigned int counter = 0;
    while (counter < max_counter_val) {
        const data_chunk data{counter};
        std::cout << "data_preparation_thread() : data_chunk data=" << counter
                  << "\n";
        data_queue.push(data);
        counter++;
    }
}

void data_processing_thread() {
    unsigned int counter = 0;
    while (true) {
        data_chunk data;
        data_queue.wait_and_pop(data);
        counter++;
        process(data);
        if (counter >= max_counter_val) {
            break;
        }
    }
}

int main() {

    std::thread t1(data_preparation_thread);
    std::thread t2(data_processing_thread);

    t1.join();
    t2.join();

    return 0;

    return 0;
}