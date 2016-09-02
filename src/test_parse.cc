#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "gg2aa.h"
#include "parsers.h"

int main(int, char *argv[]) {
    const std::string appname("parse_test");

    auto infile = std::make_unique<std::ifstream>(argv[1]);
    if (!infile->good()) { return failedToRead(appname, argv[1]); }

    auto fres = gg2aa::parseFitResults(std::move(infile));
    for (const auto &fr : fres) { std::cout << fr << '\n'; }
}
