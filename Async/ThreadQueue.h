#pragma once

#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>

// send jobs to be run asynchronously by worker threads
class ThreadQueue {
public:
    enum class Priority {
        VERY_LOW,
        LOW,
        NORMAL,
        HIGH,
        VERY_HIGH
    };

    // singleton
    static ThreadQueue& get_instance();

    void push(std::function<void(void)> func, Priority priority);

    ~ThreadQueue();

private:
    ThreadQueue();

    ThreadQueue(const ThreadQueue&) = delete;
    void operator=(const ThreadQueue&) = delete;

    void worker_routine();

    const int THREAD_COUNT = 2;

    std::atomic<bool> is_terminating;

    struct JobHolder {
        std::function<void(void)> func;
        Priority priority;

        bool operator<(const JobHolder& other) const {
            return this->priority < other.priority;
        }
    };
    std::priority_queue<JobHolder> queue;
    std::vector<std::thread> workers;

    std::mutex mutex;
    std::condition_variable cv;
};

