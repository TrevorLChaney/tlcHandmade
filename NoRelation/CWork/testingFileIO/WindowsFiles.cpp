#include <iostream>
#include <windows.h>

int main(int argc, const char** argv, const char** env) {
    HANDLE handle = CreateFile(argv[1], GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

    if(handle) {
        std::cout << "Created File: " << argv[1] << std::endl;
        CloseHandle(handle);
    } else {
        std::cerr << "Failed to create file: " << argv[1] << std::endl;
    }
}
