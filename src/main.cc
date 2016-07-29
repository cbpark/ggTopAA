#include "main.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include "inputdata_type.h"
#include "parsers.h"

int main(int argc, char *argv[]) {
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

    gg2aa::InputData data;

    std::istringstream test_str1("signal: data/Tab_173.0_1.50.dat");
    data = gg2aa::parseInputData(&test_str1);
    std::cout << "-- test1\n" << data.show() << '\n';

    std::istringstream test_str2("sigma:\n  data/Sigma.dat");
    data = gg2aa::parseInputData(&test_str2);
    std::cout << "-- test2\n" << data.show() << '\n';

    std::istringstream test_str3(
        "  direct:\n    - data/direct.dat\n    - data/direct.dat2");
    data = gg2aa::parseInputData(&test_str3);
    std::cout << "-- test3\n" << data.show() << '\n';

    std::istringstream test_str4("  direct:\n    - data/direct.dat\n    - "
                                 "data/direct.dat2\n  fragment:\n  - "
                                 "data/OneF.dat");
    data = gg2aa::parseInputData(&test_str4);
    std::cout << "-- test4\n" << data.show() << '\n';
}
