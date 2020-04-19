/*
 * ThreadQueue takes Jobs and runs them on a pool of worker threads.
 * Number of workers is set to number of available cores.
 * Gravitate toward using weak references when accessing shared state.
 * 
 * NOTE: Jobs should not influence the state of the simulation.
 * Use Events for that.
 */

#pragma once

#include "Job.h"

#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadQueue {
public:
    ThreadQueue();

    ThreadQueue(ThreadQueue&) = delete;
    void operator=(ThreadQueue&) = delete;

    enum class Priority {
        NORMAL,
        HIGH
    };

    // this is thread safe
    void push(Job_t job, Priority priority = Priority::NORMAL);

    ~ThreadQueue();

private:
    void worker_routine();

    std::atomic<bool> is_terminating;

    struct JobHolder {
        Job_t job;
        Priority priority;

        bool operator<(const JobHolder& other) const {
            return priority < other.priority;
        }
    };
    std::priority_queue<JobHolder> queue;
    std::vector<std::thread> workers;

    std::mutex mutex;
    std::condition_variable cv;
};