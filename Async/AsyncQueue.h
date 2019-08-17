#pragma once

#include <queue>
#include <functional>
#include <mutex>
#include <iostream>
#include <chrono>

// send small tasks to be run asynchronously on main thread in game loop
class AsyncQueue {
public:

    // singleton
    static AsyncQueue& get_instance();

    // this can be done from any thread
    void push(std::function<void(void)> func);

    // this should only be done from the main thread in game loop
    void process_all_tasks();

    //// this should only be done from the main thread in game loop
    void process_tasks_for(int duration_ms);

private:
    AsyncQueue() = default;

    AsyncQueue(const AsyncQueue&) = delete;
    void operator=(const AsyncQueue&) = delete;

    std::queue<std::function<void(void)>> queue;

    std::mutex mutex;
};

