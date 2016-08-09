#include "gg2aa.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include "fit.h"
#include "histogram.h"
#include "info.h"
#include "parsers.h"

using std::string;

const double BINSIZE_SIG = 0.25;

int main(int argc, char *argv[]) {
    const string appname = "gg2aa";
    if (argc != 3) {
        std::cerr << "Usage: " << appname << " input output\n"
                  << "    input - input file\n"
                  << "    output - output file\n"
                  << "    ex) " << appname << " input.yml output.dat\n";
        return 1;
    }
    auto infile = std::make_unique<std::ifstream>(argv[1]);
    if (!infile->good()) { return failedToRead(appname, argv[1]); }
    const auto to_out = &std::cout;

    // Parse the list of input data.
    auto data = gg2aa::parseInputData(std::move(infile));
    data.show(to_out);

    // Check the input files.
    const auto check = data.check_input();
    if (check.first != 0) {
        for (const auto &f : check.second) { failedToRead(appname, f); }
        return 1;
    }

    // Get info.
    auto info = std::make_shared<gg2aa::Info>(getInfo(data));
    if (info->status != 0) { return errMsg(appname, "info cannot be found."); }
    info->show(to_out);

    auto hists = std::make_shared<gg2aa::HistObjs>(*info, BINSIZE_SIG);
    // Fill histograms and set scales (caution: info will be updated).
    hists->fill_hists(data, info);
    info->show_bg_summary(to_out);  // Print out information of backgrounds.
    data.set_templates(*info);      // Set all the templates.

    // Create the pseudo-experiment histogram.
    message(appname, "generating pseudo-experiment data...", to_out);
    const auto h_pseudo = hists->pseudo_experiment(*info);
    message(appname, "... done.", to_out);

    // Open output file.
    const string outfile_name(argv[2]);
    auto outfile = std::make_unique<std::ofstream>(outfile_name);
    if (!outfile->good()) {
        return errMsg(appname, "failed to create `" + outfile_name + "'");
    }
    message(appname, "output will be saved to `" + outfile_name + "'", to_out);

    // Perform fitting and calculate the chi square.
    message(appname, "starting fitting...", to_out);
    for (const auto &t : data.templates()) {
        const gg2aa::FitFunction ffnc(t, *info);  // Prepare the fit function
                                                  // based on the template.
        auto fit = gg2aa::Fit(ffnc, *info);
        const double chi2 = fit.get_chisquare(h_pseudo);
        *outfile << std::fixed << std::setw(9) << std::setprecision(2)
                 << t.mass_width().first;
        *outfile << std::setw(8) << std::setprecision(2)
                 << t.mass_width().second;
        *outfile << std::setw(12) << std::setprecision(4) << chi2 << '\n';
    }
    message(appname, "... gracefully done.", to_out);
}
