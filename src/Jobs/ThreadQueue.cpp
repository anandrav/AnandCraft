#include "ThreadQueue.h"

#include <iostream>

using namespace std;

const int AVAILABLE_CORES = std::thread::hardware_concurrency() - 1; // leave one core for main thread
const int NUM_THREADS = clamp(AVAILABLE_CORES, 1, 3); // >= 1 in thread pool, <= 3 because synchronization is slow

ThreadQueue::ThreadQueue() : workers(NUM_THREADS) {
    is_terminating.store(false);

    for (int i = 0; i < NUM_THREADS; ++i) {
        workers[i] = std::thread(&ThreadQueue::worker_routine, this);
    }
}

void ThreadQueue::push(Job_t job, Priority priority) {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push({ job, priority });
    cv.notify_one();
}

void ThreadQueue::worker_routine() {
    std::unique_lock<std::mutex> lock(mutex);
    while (true) {
        // wait until there's a job or 
        while (queue.empty() && !is_terminating.load()) {
            cv.wait(lock);
        }
        if (is_terminating.load())
            return;

        auto holder = queue.top();
        queue.pop();
        lock.unlock();

        holder.job();

        lock.lock();
    }
}

ThreadQueue::~ThreadQueue() {
    is_terminating.store(true);
    cv.notify_all();
    for (size_t i = 0; i < workers.size(); ++i) {
        workers[i].join();
    }
}