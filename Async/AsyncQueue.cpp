#include "AsyncQueue.h"

AsyncQueue& AsyncQueue::get_instance() {
    static AsyncQueue instance;
    return instance;
}

void AsyncQueue::push(std::function<void(void)> func) {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(func);
}

void AsyncQueue::process_tasks() {
    std::lock_guard<std::mutex> lock(mutex);
    while (!queue.empty()) {
        auto task = queue.front();
        queue.pop();

        task();
    }
}