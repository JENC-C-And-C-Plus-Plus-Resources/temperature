#include "memory.h"

int main() {

    Memory mem(4);
    mem.write_16bit(0, 550);
    mem.write_8bit (2, 50);
    mem.write_8bit (3, 100);


    cout << "At 0xFF00: " << (static_cast<unsigned int>(mem.read_16bit(0))) << endl;
    cout << "At 0xFF02: " << (static_cast<unsigned int>(mem.read_8bit(2))) << endl;
    cout << "At 0xFF03: " << (static_cast<unsigned int>(mem.read_8bit(3))) << endl;
    return 0;
}