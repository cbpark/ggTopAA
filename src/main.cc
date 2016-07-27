#include "main.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char* argv[]) {
    std::string appname = "test";
    if (argc != 2) {
        std::cerr << " Usage: " << appname << " input\n"
                  << "     input - input file\n";
        return 1;
    }

    std::ifstream infile(argv[1]);
    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::cout << iss.str() << '\n';
    }
    infile.close();
}
