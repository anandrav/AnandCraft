#pragma once

#include <queue>
#include <functional>
#include <mutex>
#include <iostream>
#include <chrono>

// send small tasks to be run asynchronously on main thread in game loop
class AsyncQueue {
public:
    enum class Priority {
        VERY_LOW,
        LOW,
        NORMAL,
        HIGH,
        VERY_HIGH
    };

    // singleton
    static AsyncQueue& get_instance();

    // this can be done from any thread
    void push(std::function<void(void)> func, Priority priority);

    // this should only be done from the main thread in game loop
    void process_all_tasks();

    //// this should only be done from the main thread in game loop
    void process_tasks_for(int duration_ms);

private:
    AsyncQueue() = default;

    AsyncQueue(const AsyncQueue&) = delete;
    void operator=(const AsyncQueue&) = delete;

    struct JobHolder {
        std::function<void(void)> func;
        Priority priority;

        bool operator<(const JobHolder& other) const {
            return this->priority < other.priority;
        }
    };
    std::priority_queue<JobHolder> queue;

    std::mutex mutex;
};

