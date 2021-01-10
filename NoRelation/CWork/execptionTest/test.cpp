#include <iostream>
#include <cctype>

void getNumber() {
	char x = 0;
	std::cout << "x = ";
	std::cin >> x;
	if(!isdigit(x)) {
		throw "\nx is not a number";
	}
	std::cout << "\nx is " << x << std::endl;
}

int main(int argc, const char **argv, const char **env) {
	try {
		getNumber();
	} catch(const char *errorMessage) {
		std::cerr << errorMessage << std::endl;
	}
}
