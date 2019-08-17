#include "AsyncQueue.h"

AsyncQueue& AsyncQueue::get_instance() {
    static AsyncQueue instance;
    return instance;
}

void AsyncQueue::push(std::function<void(void)> func, Priority priority) {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(JobHolder{ func, priority });
}

void AsyncQueue::process_all_tasks() {
    while (!queue.empty()) {
        JobHolder job;
        {
            std::lock_guard<std::mutex> lock(mutex);
            job = queue.top();
            queue.pop();
        }

        job.func();
    }
}

void AsyncQueue::process_tasks_for(int duration) {
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;

    auto start = Time::now();
    int elapsed = 0;

    while (!queue.empty() && elapsed <= duration) {
        JobHolder job;
        {
            std::lock_guard<std::mutex> lock(mutex);
            job = queue.top();
            queue.pop();
        }

        job.func();

        auto now = Time::now();
        elapsed = (int)std::chrono::duration_cast<ms>(now - start).count();
    }
}