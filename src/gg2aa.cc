#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "inputdata_type.h"
#include "parsers.h"

int main(int argc, char *argv[]) {
    std::string appname = "gg2aa";
    if (argc != 2) {
        std::cerr << "Usage: " << appname << " input\n"
                  << "    input - input file\n"
                  << "    ex) " << appname << " input.yml\n";
        return 1;
    }

    std::unique_ptr<std::ifstream> infile(new std::ifstream(argv[1]));
    if (!infile->good()) {
        std::cerr << appname << ": failed to read `" << argv[1] << "'.\n";
        return 1;
    }

    gg2aa::InputData data = gg2aa::parseInputData(std::move(infile));
    std::cout << data.show();
}
