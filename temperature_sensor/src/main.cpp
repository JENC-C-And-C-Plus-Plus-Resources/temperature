#include "memory.h"
#include "scheduler.h"

using namespace std;

void check_water_level(int data) {
    cout << "Checking water level every 2 seconds at 0xFF02: " << data << endl;
};

void check_temperature(int data) {
    cout << "Checking temperature, every 6 seconds at 0xFF00: " << data << endl;
};

void heater_controller(int data) {
    cout << "Controlling heater, every 1 second at 0xFF03 " << data << endl;
}


int main() {

    Memory mem(4);
    mem.write_16bit(0, 550); // 0xFF00 Location of temperature: T +-5 Celsius
    mem.write_8bit (2, 50);  // 0xFF02 Location of water level: 0-100%
    mem.write_8bit (3, 100); // 0xFD03 Location of heater power: 0-100%
       
    Scheduler scheduler(4); 

    scheduler.schedule(Task(heater_controller, static_cast<int>(mem.read_8bit(3)), 1));
    scheduler.schedule(Task(check_water_level, static_cast<int>(mem.read_8bit(2)), 2));
    scheduler.schedule(Task(check_temperature, static_cast<int>(mem.read_16bit(0)), 6));

    scheduler.start();
    scheduler.stop();
    return 0;
}