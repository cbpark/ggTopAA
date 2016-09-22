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
#include "TCanvas.h"
#include "contours.h"
#include "parsers.h"

using std::string;
using std::to_string;

int main(int argc, char *argv[]) {
    const string appname("gg2aa_contour");
    const int n_choice = 6;
    if (argc != 4) {
        string usage = "Usage: " + appname + " input output fit_choice\n\n";
        usage += "    input      - input data file\n";
        usage += "    output     - output plot (pdf, png, jpg, ...)\n";
        usage += "    fit_choice - choice of fit function [1, ..., " +
                 to_string(n_choice) + "]\n\n";
        usage += "    ex) " + appname + " fitdata.dat output.pdf 1\n";
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
    const std::string outfile(argv[2]);
    message(appname, "generated plot will be saved to `" + outfile + "'.",
            to_out);

    // Result of fitting.
    auto fres = gg2aa::parseFitResults(std::move(infile));
    message(appname, "`" + std::string(argv[1]) + "' has been parsed.", to_out);

    // Create the canvas.
    auto canvas = std::make_unique<TCanvas>("c", "", 0, 0, 600, 600);
    canvas->SetTicks();  // for ticks on both sides.

    gg2aa::MassWidthCont cont(fres);
    const auto chi2_int = gg2aa::chi2Interval12(fit_choice);
    gg2aa::set_cont_levels(chi2_int.first, chi2_int.second, &cont);
    cont.hist()->Draw("CONT2");

    // auto mpoint = graphMinPoint(cont);
    // mpoint->Draw("P same");

    canvas->SaveAs(outfile.c_str());
    message(appname, "... gracefully done.", to_out);
}
