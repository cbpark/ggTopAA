#include "gg2aa.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include "TF1.h"
#include "fit.h"
#include "histogram.h"
#include "info.h"
#include "parsers.h"

const double BINSIZE_SIG = 0.25;

int main(int argc, char *argv[]) {
    const std::string appname = "gg2aa";
    if (argc != 2) {
        std::cerr << "Usage: " << appname << " input\n"
                  << "    input - input file\n"
                  << "    ex) " << appname << " input.yml\n";
        return 1;
    }
    auto infile = std::make_unique<std::ifstream>(argv[1]);
    if (!infile->good()) {
        failedToRead(appname, argv[1]);
        return 1;
    }
    auto fout = &std::cout;

    // Parse the list of input data.
    auto data = gg2aa::parseInputData(std::move(infile));
    data.show(fout);

    // Check the input files.
    const auto check = data.check_input();
    if (check.first != 0) {
        for (const auto &f : check.second) { failedToRead(appname, f); }
        return 1;
    }

    // Get info.
    auto info = std::make_shared<gg2aa::Info>(getInfo(data));
    if (info->status != 0) { return errMsg(appname, "info cannot be found."); }
    info->show(fout);

    // gg2aa::HistObjs hists(*info, BINSIZE_SIG);
    auto hists = std::make_shared<gg2aa::HistObjs>(*info, BINSIZE_SIG);
    hists->fill_hists(data, info);  // Fill histograms and set scales.
    info->show_bg_summary(fout);    // Print out information of backgrounds.
    data.set_templates(*info);      // Set all the templates.

    // Create the pseudo-experiment histogram.
    auto h_pseudo = hists->pseudo_experiment(*info);

    // Prepare the fit function.
    gg2aa::FitFunction ffnc(data.templates()[1], *info);

    // Get chi square.
    auto fit = gg2aa::Fit(ffnc, *info);
    double chi = fit.get_chisquare(h_pseudo);
    std::cout << "-- chi = " << chi << '\n';

    std::cout << appname << ": gracefully done.\n";
}
