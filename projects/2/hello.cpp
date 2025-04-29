#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <name> <number>" << std::endl;
        return 1;
    }

    std::string name = argv[1];
    int count = std::atoi(argv[2]);

    for (int i = 0; i < count; ++i) {
        std::cout << "Hello " << name << std::endl;
    }

    return 0;
}
