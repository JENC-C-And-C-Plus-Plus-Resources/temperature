#ifndef MEMORY_H
#define MEMORY_H
#include <iostream>
#include <cstdint> 
using namespace std;
/**
 * Permissions for Data management at register level.
 */
enum Permissions {
    READ,
    WRITE,
    READ_WRITE
};

/**
 * Hold unsigned int of 8 bits
 */
struct Register {
    uint8_t data = 0x0;
    Permissions permission = Permissions::READ_WRITE;
};

/**
 * Wrapps an array of registers
 */
class Memory {
private:
    Register* registers;
    size_t blocks = 0;
public:
    Memory(size_t blocks);

    /**
     * Reads an 8 bit block
     */
    const uint8_t read_8bit(size_t block);
    /**
     * Reads a 16 bit block
     */
    const uint16_t read_16bit(size_t block);

    /**
     * Writes an 8 bit block
     */
    void write_8bit(size_t block, uint8_t data);
    /**
     * Writes 16 bits data
     */
    void write_16bit(size_t block, uint16_t data);
    
    /**
     * Configures permissions at register level
     */
    void set_permission(size_t block, Permissions permission);
};
#endif