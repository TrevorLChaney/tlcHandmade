#include <iostream>
#include <cmath>

int main() {
    char x = 127;
    std::cout << std::pow(2, 8.0) << " values" << std::endl;
    for(int i = std::pow(2, 7.0) - 1; i > -128; --i) {
        if(!(i%16)) std::cout << std::endl;
        std::cout << i << ' ' << x-- << ' ';
    }
}
