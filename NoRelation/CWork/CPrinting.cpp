#include <math.h>
#include <stdio.h>
#include <vector>

int main(int argc, char **argv) {
    std::vector<char> words;
    for(char *i = argv[1]; *i != 0; i++) {
        words.push_back(*i);
    }

    int space = 0;
    for(auto word : words) {
        for(int j = 0; j < space; ++j) {
            printf(" ");
        }
        ++space;
        printf("%c\n", word);
    }
}
