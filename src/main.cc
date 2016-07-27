#include "main.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int numInitSpaces(const std::string& str) {
    int num = 0;
    for (const auto& c: str) {
        if (std::isspace(c)) {
            ++num;
        }
    }
    return num;
}

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
        if (line.find("#") == std::string::npos) {
            std::istringstream iss(line);
            std::cout << iss.str() << '\n';
        } else {
            continue;
        }
    }
    infile.close();

    std::istringstream test_str1("sigma: data/Sigma.dat");
    std::string parsed;
    while (std::getline(test_str1, parsed, ':')) {
        parsed.erase(std::remove(parsed.begin(), parsed.end(), ' '), parsed.end());
        std::cout << parsed << '\n';
    }

    std::istringstream test_str2("sigma:\n  data/Sigma.dat");
    while (std::getline(test_str2, parsed)) {
        std::cout << numInitSpaces(parsed) << "  " << parsed << '\n';
    }
}
