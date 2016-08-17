/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#include "gg2aa.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include "fit.h"
#include "histogram.h"
#include "info.h"
#include "parsers.h"

using std::to_string;

const double BINSIZE_SIG = 0.25;  // the number of bins will be 4 * (max - min).

int main(int argc, char *argv[]) {
    const std::string appname("gg2aa");
    if (argc != 3) {
        std::cerr << "Usage: " << appname << " input output\n"
                  << "    input - input file\n"
                  << "    output - output file\n"
                  << "    ex) " << appname << " input.yml output.dat\n";
        return 1;
    }
    auto infile = std::make_unique<std::ifstream>(argv[1]);
    if (!infile->good()) { return failedToRead(appname, argv[1]); }
    const auto to_out = &std::cout;  // information will be displayed in screen.

    // Parse the list of input data.
    auto data = gg2aa::parseInputData(std::move(infile));
    data.show(to_out);

    // Check the input files.
    const auto check = data.check_input();
    // If any input file fails to be read, print out the file name and exit.
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
    hists->fill(data, info);
    info->show_bg_summary(to_out);  // Print out information of backgrounds.

    message(appname, "setting templates ...", to_out);
    data.set_templates(*info);
    message(appname, "... done.", to_out);

    // Create the pseudo-experiment histogram.
    message(appname, "generating pseudo-experiment data ...", to_out);
    const auto h_pseudo = hists->pseudo_experiment(*info);
    message(appname, "... done.", to_out);

    // Open output file.
    const std::string outfile_name(argv[2]);
    auto outfile = std::make_shared<std::ofstream>(outfile_name);
    if (!outfile->good()) {
        return errMsg(appname, "failed to create `" + outfile_name + "'.");
    }
    message(appname, "output will be saved to `" + outfile_name + "'.", to_out);

    // Perform fitting and obtain the chi square.
    message(appname, "performing fitting ...", to_out);
    for (const auto &t : data.templates()) {
        const gg2aa::FitFunction ffnc(t, *info);  // Prepare the fit function
                                                  // based on the template.
        auto fit = gg2aa::Fit(ffnc);
        const double chi2 = fit.get_chisquare(h_pseudo);
        message(appname, "mass = " + to_string(t.mass_width().first) +
                             ", width = " + to_string(t.mass_width().second) +
                             ", chi2 = " + to_string(chi2),
                to_out);
        writeChiSquare(t, chi2, outfile);
    }
    outfile->close();
    message(appname, "... gracefully done.", to_out);
}
