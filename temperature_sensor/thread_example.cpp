#include <string>
#include <iostream>
#include <thread>

using namespace std;

// The function we want to execute on the new thread.
void task1(string msg)
{
    int times = 0;
    while(times < 5){
        cout << "task1 says: " << msg << endl;
        times++;
    }
}

void task2(string msg)
{
    int times = 0;
    while(times < 10){
        cout << "task2 says: " << msg << endl;
        times++;
    }
}


int main()
{
    // Constructs the new thread and runs it. Does not block execution.
    thread t1(task1, "Hello");
    t1.join();
    thread t2(task2, "World");
    t2.join();

    // Do other things...

    // Makes the main thread wait for the new thread to finish execution, therefore blocks its own execution.
}