#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

static unsigned int data_counter = 0; // keep track of num data points generated
const unsigned int max_counter = 10;  // maximum data points to generate

struct data_chunk {
    unsigned int data;
};

// data point generator function
data_chunk prepare_data() {
    data_counter += 1;

    std::cout << "prepare_data() : preparing data_chunk with data="
              << data_counter << "\n";

    return data_chunk{data_counter};
}

// check if more data to generate
bool more_data_to_prepare() {
    return data_counter < max_counter;
}

// check if last data point
bool is_last_chunk(data_chunk& chunk) {
    return chunk.data == max_counter;
}

// function to process data
void process(data_chunk& chunk) {
    std::cout << "process() : data_chunk data=" << chunk.data << "\n";
}

std::mutex mut;
std::queue<data_chunk> data_queue;
std::condition_variable data_cond;

// producer thread
void data_preparation_thread() {
    std::cout << "Starting data_preparation_thread()\n";

    while (more_data_to_prepare()) {
        const data_chunk data = prepare_data();
        std::lock_guard<std::mutex> lk(mut);
        // If you comment out following line, consumer thread will be in blocked
        // state forever. Thread sanitizer cannot catch this condition. There
        // are no data races or deadlocks! Code is logically wrong.
        data_queue.push(data);
        data_cond.notify_one();
    }

    std::cout << "Exiting data_preparation_thread()\n";
}

// consumer thread
void data_processing_thread() {
    std::cout << "Starting data_processing_thread()\n";

    while (true) {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, []() { return !data_queue.empty(); });

        data_chunk data = data_queue.front();
        data_queue.pop();
        lk.unlock();

        process(data);
        if (is_last_chunk(data)) {
            break;
        }
    }

    std::cout << "Exiting data_processing_thread()\n";
}

int main() {

    std::thread t1(data_preparation_thread);
    std::thread t2(data_processing_thread);

    t1.join();
    t2.join();

    return 0;
}