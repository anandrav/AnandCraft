#include "DispatchQueue.h"

DispatchQueue& DispatchQueue::get_instance() {
    static DispatchQueue instance;
    return instance;
}

void DispatchQueue::push(std::function<void(void)> func) {
    {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(func);
    }

    cv.notify_one();
}

DispatchQueue::DispatchQueue() : workers(THREAD_COUNT) {
    is_terminating.store(false);

    for (int i = 0; i < THREAD_COUNT; ++i) {
        workers[i] = std::thread(&DispatchQueue::worker_routine, this);
    }
}

void DispatchQueue::worker_routine() {
    std::unique_lock<std::mutex> lock(mutex);

    while (!is_terminating.load()) {
        // wait until notified that a job was pushed
        cv.wait(lock);
        
        // in case of spurious wakeup or destructor
        if (!queue.empty()) {
            auto func = std::move(queue.front());
            queue.pop();

            lock.unlock();
            func();
            lock.lock();
        }
    }
}

DispatchQueue::~DispatchQueue() {
    is_terminating.store(true);
    cv.notify_all();

    for (int i = 0; i < workers.size(); ++i) {
        std::cout << "join attempt " << i << std::endl;
        workers[i].join();
        std::cout << "join success " << i << std::endl;
    }
}