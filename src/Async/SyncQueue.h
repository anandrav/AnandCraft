#pragma once

#include <queue>
#include <functional>
#include <mutex>

// send small tasks to be run on main thread in game loop
// used by the ThreadQueue to send messages/results
class SyncQueue {
public:

    // singleton
    static SyncQueue& get_instance();

    // this can be done from any thread
    void push(std::function<void(void)> func);

    // this should only be done from the main thread in game loop
    void process_all_tasks();

    // this should probably be deprecated
    // this should only be done from the main thread in game loop
    void process_tasks_for(int duration);

private:
    SyncQueue() = default;

    SyncQueue(const SyncQueue&) = delete;
    void operator=(const SyncQueue&) = delete;

    std::queue<std::function<void(void)>> queue;

    std::mutex mutex;
};

