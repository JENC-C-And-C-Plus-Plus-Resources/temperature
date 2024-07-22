#include "scheduler.h"

Task::Task(std::function<void(int)> func,int data, int period)
        : func(func), period(period), data(data) {}


Scheduler::Scheduler(size_t numThreads) : stopFlag(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        threads.emplace_back(&Scheduler::worker, this);
    }
}

Scheduler::~Scheduler() {
    stop();
}

void Scheduler::schedule(const Task& task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        taskQueue.push(task);
    }
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
        Task task([](int){}, 0, 0);
        {
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