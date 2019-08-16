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
    while (!queue.empty()) {
        std::function<void(void)> task;
        {
            std::lock_guard<std::mutex> lock(mutex);
            task = queue.front();
            queue.pop();
        }

        task();
    }
}