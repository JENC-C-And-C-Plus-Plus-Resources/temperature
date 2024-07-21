#include <iostream>
#include <bitset>
 
int main() {
    uint8_t a = 50;
    std::cout << a <<  " "<< static_cast<unsigned int>(a) << '\n';
    std::cout << a <<  " "<< static_cast<int>(a) << '\n';
    return 0;
};


