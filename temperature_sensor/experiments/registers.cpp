#include <iostream>
#include <cstdint>

using namespace std;

class Register{
    uint8_t reg_ax[2];
public:
    Register(){
        reg_ax[0] = reg_ax[1] = 0;
    }
    uint16_t &ax(){ return (uint16_t &)reg_ax[0]; }
    uint8_t &al(){ return reg_ax[1]; }
    uint8_t &ah(){ return reg_ax[0]; }
};


int main()
{
    Register regs;
    regs.ax() = 500;
    cout << regs.ax() << endl;
    cout << (int)regs.ah() << endl;
    cout << (int)regs.al() << endl;
    return 0;
}