#include "ThreadQueue.h"

ThreadQueue& ThreadQueue::get_instance() {
    static ThreadQueue instance;
    return instance;
}

void ThreadQueue::push(std::function<void(void)> func) {
    {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(func);
    }

    cv.notify_one();
}

ThreadQueue::ThreadQueue() : workers(THREAD_COUNT) {
    is_terminating.store(false);

    for (int i = 0; i < THREAD_COUNT; ++i) {
        workers[i] = std::thread(&ThreadQueue::worker_routine, this);
    }
}

void ThreadQueue::worker_routine() {
    std::unique_lock<std::mutex> lock(mutex);

    int x = 0;
    while (!is_terminating.load()) {
        // wait until notified that a job was pushed
        cv.wait(lock, [this]() {
                return !queue.empty() || is_terminating.load();
            }
        );
        
        // in case of spurious wakeup or destructor
        if (!queue.empty()) {
            auto func = std::move(queue.front());
            queue.pop();
            std::cout << "popped " << x++ << std::endl;
            std::cout << "queue size: " << queue.size() << std::endl;


            lock.unlock();
            func();
            lock.lock();
        }
    }
}

ThreadQueue::~ThreadQueue() {
    is_terminating.store(true);
    cv.notify_all();

    for (int i = 0; i < workers.size(); ++i) {
        std::cout << "join attempt " << i << std::endl;
        workers[i].join();
        std::cout << "join success " << i << std::endl;
    }
}