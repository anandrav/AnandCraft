#include "AsyncQueue.h"

AsyncQueue& AsyncQueue::get_instance() {
    static AsyncQueue instance;
    return instance;
}

void AsyncQueue::push(std::function<void(void)> func) {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(func);
}

void AsyncQueue::process_all_tasks() {
    std::lock_guard<std::mutex> lock(mutex);
    while (!queue.empty()) {
        auto job = queue.front();
        queue.pop();
        job();
    }
}

//void AsyncQueue::process_tasks_for(int duration) {
//    typedef std::chrono::high_resolution_clock Time;
//    typedef std::chrono::milliseconds ms;
//
//    auto start = Time::now();
//    int elapsed = 0;
//
//    while (!queue.empty() && elapsed <= duration) {
//        std::function<void(void)> job;
//        {
//            std::lock_guard<std::mutex> lock(mutex);
//            job = queue.front();
//            queue.pop();
//        }
//
//        job();
//
//        auto now = Time::now();
//        elapsed = (int)std::chrono::duration_cast<ms>(now - start).count();
//    }
//}