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
    // singleton
    static ThreadQueue& get_instance();

    void push(std::function<void(void)> func);

    ~ThreadQueue();

private:
    ThreadQueue();

    ThreadQueue(const ThreadQueue&) = delete;
    void operator=(const ThreadQueue&) = delete;

    void worker_routine();

    const int THREAD_COUNT = 2;

    std::atomic<bool> is_terminating;

    std::queue<std::function<void(void)>> queue;
    std::vector<std::thread> workers;

    std::mutex mutex;
    std::condition_variable cv;
};

