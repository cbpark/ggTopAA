/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#include "gg2aa.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include "fit.h"
#include "histogram.h"
#include "info.h"
#include "parsers.h"
#include "templates.h"

using std::string;
using std::to_string;

const double BINSIZE_SIG = 0.25;  // the number of bins will be 4 * (max - min).

int main(int argc, char *argv[]) {
    const string appname("gg2aa");
    const int n_choice = 6;
    if (argc < 4 || argc > 5) {
        string usage =
            "Usage: " + appname + " input output fit_choice [best_fit]\n\n";
        usage += "    input      - input file (yml)\n";
        usage += "    output     - output file for the fit result\n";
        usage += "    fit_choice - choice of fit function [1, ..., " +
                 to_string(n_choice) + "]\n";
        usage += "    best_fit   - output file for the best-fit point "
                 "(optional, will be appended)\n\n";
        usage += "    ex) " + appname + " input.yml output.dat 1 best.dat\n";
        return howToUse(usage);
    }

    const int fit_choice = std::atoi(argv[3]);
    if (!correctChoice(fit_choice, n_choice)) {
        return errMsg(appname, "fit_choice must be in [1, ..., " +
                                   to_string(n_choice) + "].");
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
    // The boolean parameter is to reset the random number generator.
    const auto h_pseudo = hists->pseudo_experiment(*info, true);
    message(appname, "... done.", to_out);

    // Open output file.
    const string out_fit_name(argv[2]);
    auto out_fit = std::make_shared<std::ofstream>(out_fit_name);
    if (!out_fit->good()) {
        return errMsg(appname, "failed to create `" + out_fit_name + "'.");
    }
    message(appname, "output of fit will be saved to `" + out_fit_name + "'.",
            to_out);
    write_header(out_fit.get());

    gg2aa::BestFitPoint best;  // the best-fit point

    // Perform fitting and obtain the chi square.
    message(appname, "performing fitting ...", to_out);
    for (const auto &t : data.templates()) {
        message(appname, "template mass = " + to_string(t.mass_width().first) +
                             ", width = " + to_string(t.mass_width().second),
                to_out);
        auto result = std::make_shared<gg2aa::FitResult>(t);
        auto fit = gg2aa::mkFit(t, *info, fit_choice);
        fit.do_fit(h_pseudo, result);
        *out_fit << *result << '\n';

        best.set_best_point(*result);  // Save the best-fit point.
    }
    out_fit->close();

    *to_out << '\n';
    best.show(to_out);  // the best-fit point is shown to the screen
    if (argc == 5) {    // ... and saved to a file.
        const string out_best_name(argv[4]);
        message(appname, "the best-fit point will be appended to `" +
                             out_best_name + "'.",
                to_out);
        auto out_best =
            std::make_unique<std::ofstream>(out_best_name, std::ios::app);
        if (!out_best->good()) {
            return errMsg(appname, "failed to open `" + out_best_name + "'.");
        }
        *out_best << best << '\n';
        out_best->close();
    }

    message(appname, "... gracefully done.", to_out);
}
