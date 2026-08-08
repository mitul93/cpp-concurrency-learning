#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

template <typename T>
class threadsafe_queue {
  private:
    std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;

  public:
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