#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
    char *z = argv[argc - 2] + '\0';
    char *astr = argv[argc - 1] + '\0';
    char result[100];

    if(argc < 4) {
        std::cout << "This program needs three arguments passed\n"
              << "\nExample: <program> <filename> <string1> <string2>\n"
              << "<program> - name of this program.\n"
              << "<filename> - name of the file in which the output will be stored.\n"
              << "<string> - a string you with to be xor encripted."
              << std::endl;
        exit(0);
    }
    std::fstream textFile(argv[argc - 3], 'w');

    if(!(textFile.is_open())) {
        std::cout << "File Error: File failed to open." << std::endl;
    }

    std::cout << "argc: " << argc << '\n';

    for(int i = 0; i <= argc - 1; ++i) {
        std::cout << "argv" << i << ": " << argv[i] << '\n';
    }

    for(int i = 0; argv[argc - 2][i] != '\0'; ++i) {
        std::cout << (int)argv[argc - 2][i] << ' ';
        result[i] = z[i] ^ astr[i];
        result[i+1] = '\0';
    }

    std::cout << "\n------------------------\n" << std::endl;
    std::cout << result << std::endl;

    for(int i = 0; result[i] != '\0'; ++i) {
        std::cout << result[i] << '\n';
    }

    textFile << result;
    textFile.close();
}
