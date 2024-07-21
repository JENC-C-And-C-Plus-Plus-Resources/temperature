#include <iostream>
#include <functional>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

class Task {
public:
    Task(std::function<void()> func, int period)
        : func(func), period(period) {}

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
        Task task([]{}, 0);
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return !taskQueue.empty() || stopFlag; });
            if (stopFlag && taskQueue.empty()) {
                return;
            }
            task = taskQueue.front();
            taskQueue.pop();
        }


        task.func();

        if (task.period > 0) {
            std::this_thread::sleep_for(std::chrono::seconds(task.period));
            schedule(task);  // Reschedule the task for periodic execution
        }
    }
}


void check_water_level() {
    std::cout << "Water level, every 2 seconds" << std::endl;
};

void check_temperature() {
    std::cout << "Temperature, every 6 seconds" << std::endl;
};


int main() {
    Scheduler scheduler(4); 

    scheduler.schedule(Task(check_water_level, 2));
    scheduler.schedule(Task(check_temperature, 6));


    // Start the scheduler
    scheduler.start();
       
    // Let the scheduler run for a while
    //std::this_thread::sleep_for(std::chrono::seconds(10));

    // Stop the scheduler
    scheduler.stop();

    return 0;
}