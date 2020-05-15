/*
 * SyncQueue is to be used in conjunction with ThreadQueue when
 * the results of a Job need to be synced on the main thread,
 * OpenGL calls in particular.
 * 
 * NOTE: Jobs should not influence the state of the simulation.
 * Use Events for that.
 */

#pragma once

#include "Job.h"

#include <queue>
#include <mutex>

class SyncQueue {
public:
    // this is thread-safe
    void push(Job_t job);

    // run this from the main thread in game loop
    void process_all();

private:
    std::queue<Job_t> queue;
    // triple mutex pattern, main thread gets priority
    std::mutex data_mutex;
    std::mutex next_mutex;
    std::mutex low_priority_mutex;
};