/**
 * 
 * Problem statement:
    Provide a solution to control the temperature of a liquid in a container. 
    The temperature target is 100 C +- 5 C. 
    If the liquid level falls below half-full the heating shall be turned off, if it rises above half-full, the heating shall be turned on.
    There is a sensor that samples the temperature of the liquid every 6 seconds. 
    This sensor is mapped to a 16-bit read-only hardware register that can be accessed directly at the memory location 0xFF00. The values at the register are in Celsius.
    There is a sensor that samples the level of the liquid every 2 seconds. 
    This sensor is mapped to an 8-bit read-only hardware register that can be accessed directly at the memory location 0xFF02. The values at the register are in percentages (0-100%).
    There is a heating element that can be used to control the temperature of the liquid. 
    This heating element is mapped to an 8-bit read-write hardware register that can be accessed directly at the memory location 0xFF03. The values at the register are in percentages (0-100%). The percentages represent how much power is applied to the heating element.
    Write the code to create a control loop for the problem statement above.
    You can assume further details as long as you document them in your justification.
 */



#include <iomanip>
#include "memory.h"
#include "scheduler.h"

#define TEMP_LEVEL_REGISTER 0
#define WATER_LEVEL_REGISTER 2
#define HEATER_POWER_REGISTER 3
#define STATUS_REGISTER 4

enum STATUS {
    HEATER_TURNED_OFF,
    HEATER_ON,
};


using namespace std;

void check_water_level(void* mem)
{
    Memory* memory = (Memory*)mem;
    const uint8_t water_level = memory->read_8bit(WATER_LEVEL_REGISTER);
    const uint8_t temperature_level = memory->read_8bit(TEMP_LEVEL_REGISTER);

    if(water_level >= 51 && temperature_level < 100) 
    {
        memory->write_8bit(STATUS_REGISTER,STATUS::HEATER_ON);
    }
};

void heater_controller(void* mem)
{
    Memory* memory = (Memory*)mem;
    const uint8_t status = memory->read_8bit(STATUS_REGISTER); 
    const uint16_t temperature_level = memory->read_16bit(TEMP_LEVEL_REGISTER);

    if(status == STATUS::HEATER_ON && temperature_level < 100)
    {
        memory->write_8bit(HEATER_POWER_REGISTER,25);
    } 
    else {
        memory->write_8bit(HEATER_POWER_REGISTER,0);
        memory->write_8bit(STATUS_REGISTER, STATUS::HEATER_TURNED_OFF);
    }
};

string map_status_to_string(uint8_t status)
{
    std::string status_map[2] = { "Heater OFF", "Heater ON" };
    return status_map[status];
};

void dashboard(void* mem)
{
    Memory* memory = (Memory*)mem;
    const uint16_t temperature_level = memory->read_16bit(TEMP_LEVEL_REGISTER);
    const uint8_t water_level = memory->read_8bit(WATER_LEVEL_REGISTER);
    const uint8_t heater_level = memory->read_8bit(HEATER_POWER_REGISTER);
    const uint8_t status = memory->read_8bit(STATUS_REGISTER);

    std::cout << "\033c" << std::endl;
    std::string headers[4] = {"Temperature Celsius", "Water Level", "Heater Level", "Status"};
    // Print headers
    for (auto &header: headers) {
        std::cout << std::setw(35) << header;
    }

    std::string data[1][4] = {
        {
            std::to_string(static_cast<unsigned int>(temperature_level)) + " C", 
            std::to_string(static_cast<unsigned int>(water_level)) + " %",
            std::to_string(static_cast<unsigned int>(heater_level)) + " %",
            map_status_to_string(status)
        }
    };

    std::cout << std::endl;

    // Print data
    for (auto &row: data) {
        for (auto &column: row) {
            std::cout << std::setw(35) << column;
        }
        std::cout << std::endl;
    }
};

void external_simulator(void* mem) {
    Memory* memory = (Memory*)mem;
    const uint8_t status = memory->read_8bit(STATUS_REGISTER); 
    const uint8_t water_level = memory->read_8bit(WATER_LEVEL_REGISTER);

    if(water_level < 51)
    {
        memory->write_8bit(WATER_LEVEL_REGISTER, water_level+10);
    }
    
    if(status == STATUS::HEATER_ON)
    {
        const uint16_t temperature_level = memory->read_16bit(TEMP_LEVEL_REGISTER);
        const uint16_t heater_powrer = memory->read_8bit(HEATER_POWER_REGISTER);
        memory->write_16bit(TEMP_LEVEL_REGISTER, temperature_level + (heater_powrer/5));
    }
}

int main() {
    
    Memory mem(5);
    mem.write_16bit(TEMP_LEVEL_REGISTER, 25); // 0xFF00 Location of temperature: T +-5 Celsius
    mem.write_8bit (WATER_LEVEL_REGISTER, 0); // 0xFF02 Location of water level: 0-100%
    mem.write_8bit (HEATER_POWER_REGISTER, 0);  // 0xFF03 Location of heater power: 0-100%
    mem.write_8bit (STATUS_REGISTER, STATUS::HEATER_TURNED_OFF);  // 0xFF04 Location of the status: 0-100%
       
    Scheduler scheduler(4); 

    scheduler.schedule(Task(dashboard, &mem, 1));
    scheduler.schedule(Task(external_simulator, &mem, 1)); 
    scheduler.schedule(Task(check_water_level, &mem, 2));
    scheduler.schedule(Task(heater_controller, &mem, 6));

    scheduler.start();
    scheduler.stop();
    return 0;
}