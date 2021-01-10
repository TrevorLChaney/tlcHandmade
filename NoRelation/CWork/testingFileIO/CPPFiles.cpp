#include <iostream>
#include <fstream>

int main(int argc, const char** argv, const char** env) {
    std::ofstream file(argv[1]);
    if(file.is_open()) {
        std::cout << "Created File: " << argv[1] << std::endl;
        file.close();
    } else {
        std::cerr << "Failed to open file " << argv[1] << std::endl;
    }
}
