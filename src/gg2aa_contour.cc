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
#include "TCanvas.h"
#include "TGraph.h"
#include "TH2D.h"
#include "contours.h"
#include "parsers.h"

using std::string;

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

    // Result of fitting.
    auto fres = gg2aa::parseFitResults(std::move(infile));
    message(appname, "`" + std::string(argv[1]) + "' has been parsed.", to_out);

    gg2aa::MassWidthCont mw_cont(fres);
    const auto mpoint = gg2aa::minPoint(mw_cont);

    auto canvas = std::make_unique<TCanvas>("c", "", 600, 600);
    canvas->SetTicks();  // for ticks on both sides.

    TH2D *hist = mw_cont.graph()->GetHistogram();
    const double min_chi2 = mpoint.second;
    const double contour[2] = {min_chi2 + 4.72, min_chi2 + 9.7};
    hist->SetContour(2, contour);
    hist->Draw("CONT2");

    auto min_xy = std::make_unique<TGraph>(1);
    min_xy->SetPoint(0, mpoint.first[0], mpoint.first[1]);
    min_xy->SetMarkerStyle(20);
    min_xy->Draw("P same");

    const std::string outfile(argv[2]);
    message(appname, "generated plot will be saved to `" + outfile + "'.",
            to_out);
    canvas->SaveAs(outfile.c_str());
    message(appname, "... gracefully done.", to_out);
}
