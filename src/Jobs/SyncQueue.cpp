#include "SyncQueue.h"

using namespace std;

void SyncQueue::push(Job_t job) {
    lock_guard<mutex> lock(q_mutex);
    queue.push(job);
}

void SyncQueue::process_all() {
    lock_guard<mutex> lock(q_mutex);
    while (!queue.empty()) {
        auto job = queue.front();
        queue.pop();
        job();
    }
}