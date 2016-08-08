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
    const auto fout = &std::cout;

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

    auto hists = std::make_shared<gg2aa::HistObjs>(*info, BINSIZE_SIG);
    hists->fill_hists(data, info);  // Fill histograms and set scales.
    info->show_bg_summary(fout);    // Print out information of backgrounds.
    data.set_templates(*info);      // Set all the templates.

    // Create the pseudo-experiment histogram.
    std::cout << "\ngg2aa: generating pseudo-experiment data...\n";
    const auto h_pseudo = hists->pseudo_experiment(*info);

    std::cout << "\ngg2aa: starting fitting...\n\n";
    for (const auto &t : data.templates()) {
        // Prepare the fit function based on the template.
        const gg2aa::FitFunction ffnc(t, *info);
        auto fit = gg2aa::Fit(ffnc, *info);
        const double chi2 = fit.get_chisquare(h_pseudo);  // Get chi square.
        std::cout << "-- mass = " << t.mass_width().first
                  << ", width = " << t.mass_width().second
                  << ", chisquare = " << chi2 << '\n';
    }

    std::cout << appname << ": gracefully done.\n";
}
