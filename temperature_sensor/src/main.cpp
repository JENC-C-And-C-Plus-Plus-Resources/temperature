#include "memory.h"
#include "scheduler.h"

void check_water_level() {
    std::cout << "Water level, every 2 seconds" << std::endl;
};

void check_temperature() {
    std::cout << "Temperature, every 6 seconds" << std::endl;
};


int main() {

    Memory mem(4);
    mem.write_16bit(0, 550);
    mem.write_8bit (2, 50);
    mem.write_8bit (3, 100);


    cout << "At 0xFF00: " << (static_cast<unsigned int>(mem.read_16bit(0))) << endl;
    cout << "At 0xFF02: " << (static_cast<unsigned int>(mem.read_8bit(2))) << endl;
    cout << "At 0xFF03: " << (static_cast<unsigned int>(mem.read_8bit(3))) << endl;
    
    
    Scheduler scheduler(4); 

    scheduler.schedule(Task(check_water_level, 2));
    scheduler.schedule(Task(check_temperature, 6));

    scheduler.start();
    scheduler.stop();
    return 0;
}