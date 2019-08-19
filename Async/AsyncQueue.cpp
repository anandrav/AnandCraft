#include "AsyncQueue.h"
#include "SDL2/SDL.h"

AsyncQueue& AsyncQueue::get_instance() {
    static AsyncQueue instance;
    return instance;
}

void AsyncQueue::push(std::function<void(void)> func) {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(func);
}

void AsyncQueue::process_all_tasks() {
    // avoid acquiring the lock if possible
    if (queue.empty()) { return; }

    std::lock_guard<std::mutex> lock(mutex);
    while (!queue.empty()) {
        auto job = queue.front();
        queue.pop();
        job();
    }
}