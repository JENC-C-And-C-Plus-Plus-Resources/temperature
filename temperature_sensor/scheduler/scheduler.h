#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <iostream>
#include <functional>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

/**
 * Wraps a function to be scheduled
 */
class Task {
public:
    // structure of the function
    // void myFunc (void* data ) {
    // }
    // Data that the function will manage.
    // Period: execution period.
    Task(std::function<void(void*)> func, void* data, int period);

    std::function<void(void*)> func; // Abstraction of a function
    int period; // Time in seconds to be re scheduled.
    void* data;
};

/**
 * Wrapper that encapsulates the task scheduling.
 */
class Scheduler {
public:
    // Set how many tasks are going to be scheduled.
    Scheduler(size_t numThreads);
    ~Scheduler();

    // Function that adds to the internal scheduler.
    void schedule(const Task& task);
    
    // Initialization of tasks.
    void start();

    // Stop all tasks.
    void stop();

private:
    // This function wraps the rescheduling and execution of the task.
    void worker();

    // Data structure to store the threads.
    std::vector<std::thread> threads;
    // Data structure to store the order based on the period.
    std::queue<Task> taskQueue;
    // For locking and mutex to handle collissions between tasks
    std::mutex queueMutex;
    std::condition_variable condition;
    // to stop the tasks
    bool stopFlag;
};


#endif