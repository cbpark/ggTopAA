#include "gg2aa.h"
#include <fstream>
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
        failedToRead(appname, argv[1]);
        return 1;
    }

    // Parse the list of input data.
    gg2aa::InputData data = gg2aa::parseInputData(std::move(infile));
    std::cout << data.show();

    // Check the input files.
    auto check = data.check_input();
    if (check.first != 0) {
        for (const auto &f : check.second) { failedToRead(appname, f); }
        return 1;
    }
}
