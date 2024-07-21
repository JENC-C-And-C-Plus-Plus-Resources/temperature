#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <iostream>
#include <functional>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

class Task {
public:
    Task(std::function<void()> func, int period);

    std::function<void()> func; // Abstraction of a function
    int period; // Time in seconds to be re scheduled.
};

class Scheduler {
public:
    Scheduler(size_t numThreads);
    ~Scheduler();

    void schedule(const Task& task);
    void start();
    void stop();

private:
    void worker();

    std::vector<std::thread> threads;
    std::queue<Task> taskQueue;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stopFlag;
};


#endif