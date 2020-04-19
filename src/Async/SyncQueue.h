#pragma once

#include <queue>
#include <functional>
#include <mutex>

// sync the results of concurrent jobs with the main thread
class SyncQueue {
public:
    static SyncQueue& instance();

    SyncQueue(const SyncQueue&) = delete;
    void operator=(const SyncQueue&) = delete;
    SyncQueue(const SyncQueue&&) = delete;
    void operator=(const SyncQueue&&) = delete;

    // this is thread-safe
    void push(std::function<void(void)> func);

    // run this from the main thread in game loop, each frame
    void process_all_tasks();

private:
    SyncQueue() = default;
    ~SyncQueue() = default;

    std::queue<std::function<void(void)>> queue;

    std::mutex mutex;
};

