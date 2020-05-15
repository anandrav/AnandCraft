#include "SyncQueue.h"

using namespace std;

void SyncQueue::push(Job_t job) {
    // low priority
    unique_lock<mutex> low(low_priority_mutex);
    unique_lock<mutex> next(next_mutex);
    lock_guard<mutex> data(data_mutex);
    next.unlock();

    queue.push(job);
}

void SyncQueue::process_all() {
    // priority to main thread
    unique_lock<mutex> next(next_mutex);
    lock_guard<mutex> data(data_mutex);
    next.unlock();

    while (!queue.empty()) {
        auto job = queue.front();
        queue.pop();
        job();
    }
}