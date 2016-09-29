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
    if (argc != 3) {
        string usage = "Usage: " + appname + " input output\n\n";
        usage += "    input  - input data file\n";
        usage += "    output - output plot (pdf, png, jpg, ...)\n\n";
        usage += "    ex) " + appname + " fitdata.dat output.pdf\n";
        return howToUse(usage);
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
    gg2aa::set_cont_levels(2.29575, 6.18007, &cont);
    cont.hist()->Draw("CONT2");

    // auto mpoint = graphMinPoint(cont);
    // mpoint->Draw("P same");

    canvas->SaveAs(outfile.c_str());
    message(appname, "... gracefully done.", to_out);
}
