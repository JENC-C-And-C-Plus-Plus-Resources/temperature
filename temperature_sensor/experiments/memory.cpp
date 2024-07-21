#include <iostream>
#include <cstdint>

using namespace std;

enum Permissions {
    READ,
    WRITE,
    READ_WRITE
};

struct Register {
    uint8_t data = 0x0;
    Permissions permission = Permissions::READ_WRITE;
};

class Memory
{
private:
    Register* registers;
    size_t blocks = 0;
public:
    Memory(size_t blocks): blocks(blocks)
    {
        registers = new Register[blocks]();
    }

    uint8_t read_8bit(size_t block)
    {
        return registers[block].data;
    }

    const uint16_t read_16bit(size_t block)
    { 
        uint8_t low = registers[block].data;
        uint8_t high = registers[block+1].data;
        const uint16_t high_low = ((uint16_t)high) << 8 | low;
        return std::move(high_low); 
    }

    void write_8bit(size_t block, uint8_t data)
    {   
        if(registers[block].permission == Permissions::READ_WRITE || registers[block].permission == Permissions::WRITE  ){
            registers[block].data = data;
        }
    }

    void write_16bit(size_t block, uint16_t data)
    {
        Permissions low_permission =  registers[block].permission;
        Permissions high_permission =  registers[block+1].permission;
        write_8bit(block, data & 0x00FF);  // Extract and write lower 8 bits
        write_8bit(block+1, (data >> 8) & 0x00FF);  // Extract and write higher 8 bits
    }

    void set_permission(size_t block, Permissions permission)
    {
        registers[block].permission = permission;
    }

};


int main()
{
    Memory mem(4);
    mem.write_16bit(0, 550);
    mem.write_8bit (2, 50);
    mem.write_8bit (3, 100);


    cout << "At 0xFF00: " << (static_cast<unsigned int>(mem.read_16bit(0))) << endl;
    cout << "At 0xFF02: " << (static_cast<unsigned int>(mem.read_8bit(2))) << endl;
    cout << "At 0xFF03: " << (static_cast<unsigned int>(mem.read_8bit(3))) << endl;

    return 0;
}