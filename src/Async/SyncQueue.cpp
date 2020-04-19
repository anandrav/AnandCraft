#include "SyncQueue.h"
#include "SDL2/SDL.h"

SyncQueue& SyncQueue::instance() {
    static SyncQueue singleton;
    return singleton;
}

void SyncQueue::push(std::function<void(void)> func) {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(func);
}

void SyncQueue::process_all_tasks() {
    std::lock_guard<std::mutex> lock(mutex);
    while (!queue.empty()) {
        queue.front()();
        queue.pop();
    }
}