#include "scheduler.h"

Task::Task(std::function<void(void*)> func,void* data, int period)
        : func(func), period(period), data(data) {}


Scheduler::Scheduler(size_t numThreads) : stopFlag(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        // insertion to the back of the worker execution
        threads.emplace_back(&Scheduler::worker, this);
    }
}

Scheduler::~Scheduler() {
    stop();
}

void Scheduler::schedule(const Task& task) {
    {
        // lock the common data source queue
        std::unique_lock<std::mutex> lock(queueMutex);
        taskQueue.push(task);
    }
    // used to notify all the tasks.
    condition.notify_one();
}

void Scheduler::start() {
    stopFlag = false;
}

void Scheduler::stop() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stopFlag = true;
    }
    condition.notify_all();
    for (std::thread& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void Scheduler::worker() {
    while (true) {
        Task task([](void*){}, 0, 0);
        {
            // Race condition, lock and wait so the function can use the shared resource.
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return !taskQueue.empty() || stopFlag; });
            if (stopFlag && taskQueue.empty()) {
                return;
            }
            task = taskQueue.front();
            taskQueue.pop();
        }

        task.func(task.data);

        if (task.period > 0) {
            std::this_thread::sleep_for(std::chrono::seconds(task.period));
            schedule(task);  // Reschedule the task for periodic execution
        }
    }
}