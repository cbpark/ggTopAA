#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "fit.h"
#include "gg2aa.h"
#include "parsers.h"

int main(int, char *argv[]) {
    const std::string appname("parse_test");

    auto infile = std::make_unique<std::ifstream>(argv[1]);
    if (!infile->good()) { return failedToRead(appname, argv[1]); }

    auto fres = gg2aa::parseFitResults(std::move(infile));
    auto frfunc = gg2aa::FitResultFunc(fres);

    double mass, width;
    for (int i = 0; i != 10; ++i) {
        for (int j = 0; j != 100; ++j) {
            mass = 172 + i * 0.2;
            width = 0.5 + j * 0.01;
            double x[2] = {mass, width};
            std::cout << "interp (mass = " << mass << ", width = " << width
                      << ") = " << frfunc(x) << '\n';
        }
    }
}
