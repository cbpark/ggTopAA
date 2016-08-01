#include "gg2aa.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "histograms.h"
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
    auto data = gg2aa::parseInputData(std::move(infile));
    std::cout << data.show();

    // Check the input files.
    auto check = data.check_input();
    if (check.first != 0) {
        for (const auto &f : check.second) { failedToRead(appname, f); }
        return 1;
    }

    // Get info.
    auto info = std::make_shared<gg2aa::InputInfo>(getInputInfo(data));
    if (info->status != 0) {
        errMsg(appname, "info cannot be found.");
        return 1;
    }
    std::cout << info->show();

    gg2aa::Histograms hists(*info);
    hists.set_bg_hist(data, info);
    std::cout << info->show_sigma() << info->show_bg_summary();

    std::cout << appname << ": done.\n";
}
