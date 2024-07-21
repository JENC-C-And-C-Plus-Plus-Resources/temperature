#ifndef MEMORY_H
#define MEMORY_H
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

class Memory {
private:
    Register* registers;
    size_t blocks = 0;
public:
    Memory(size_t blocks);

    const uint8_t read_8bit(size_t block);
    const uint16_t read_16bit(size_t block);

    void write_8bit(size_t block, uint8_t data);
    void write_16bit(size_t block, uint16_t data);
    void set_permission(size_t block, Permissions permission);
};
#endif