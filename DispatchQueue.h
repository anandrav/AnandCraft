#pragma once

#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>

// send jobs to be run asynchronously by worker threads
class DispatchQueue {
public:
    // singleton
    static DispatchQueue& get_instance();

    void push(std::function<void(void)> func);

    ~DispatchQueue();

private:
    DispatchQueue();

    DispatchQueue(const DispatchQueue&) = delete;
    void operator=(const DispatchQueue&) = delete;

    void worker_routine();

    const int THREAD_COUNT = 2;

    std::atomic<bool> is_terminating;

    std::queue<std::function<void(void)>> queue;
    std::vector<std::thread> workers;

    std::mutex mutex;
    std::condition_variable cv;
};

