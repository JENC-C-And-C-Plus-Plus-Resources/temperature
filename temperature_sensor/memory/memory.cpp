#include "memory.h"

Memory::Memory(size_t blocks) 
{
    registers = new Register[blocks]();
}

const uint8_t Memory::read_8bit(size_t block)
{
    return registers[block].data;
}

const uint16_t Memory::read_16bit(size_t block)
{ 
    /**
     * Bit wise operation, a shift to hte left for the highest 
     * value and a or operation with the lowest part
     */
    uint8_t low = registers[block].data;
    uint8_t high = registers[block+1].data;
    const uint16_t high_low = ((uint16_t)high) << 8 | low;
    return std::move(high_low); 
}

void Memory::write_8bit(size_t block, uint8_t data)
{   
    if(registers[block].permission == Permissions::READ_WRITE || registers[block].permission == Permissions::WRITE  ){
        registers[block].data = data;
    }
}

void Memory::write_16bit(size_t block, uint16_t data)
{
    Permissions low_permission =  registers[block].permission;
    Permissions high_permission =  registers[block+1].permission;
    write_8bit(block, data & 0x00FF);  // Extract and write lower 8 bits
    write_8bit(block+1, (data >> 8) & 0x00FF);  // Extract and write higher 8 bits
}

void Memory::set_permission(size_t block, Permissions permission)
{
    registers[block].permission = permission;
}