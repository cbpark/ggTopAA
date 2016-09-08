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
#include "fit.h"
#include "parsers.h"

int main(int argc, char *argv[]) {
    const std::string appname("gg2aa_contour");
    if (argc != 3) {
        std::cerr << "Usage: " << appname << " input output\n\n"
                  << "    input  - input data file\n"
                  << "    output - output plot (pdf, png, jpg, ...)\n"
                  << "    ex) " << appname << " fitdata.dat output.pdf\n";
        return 1;
    }

    auto infile = std::make_unique<std::ifstream>(argv[1]);
    if (!infile->good()) { return failedToRead(appname, argv[1]); }
    const auto to_out = &std::cout;  // information will be displayed in screen.

    auto fres = gg2aa::parseFitResults(std::move(infile));

    gg2aa::FitResultFunc frfunc(fres);
    const auto point = gg2aa::minPoint(frfunc);

    auto canvas = std::make_unique<TCanvas>("canvas", "", 600, 600);
    canvas->SetTicks();  // ticks on both sides

    TH2D *hist = frfunc.graph()->GetHistogram();
    const double min_chi2 = point.second;
    const double contour[2] = {min_chi2 + 4.72, min_chi2 + 9.7};
    hist->SetContour(2, contour);
    hist->Draw("CONT2");

    auto min_xy = std::make_unique<TGraph>(1);
    min_xy->SetPoint(0, point.first[0], point.first[1]);
    min_xy->SetMarkerStyle(20);
    min_xy->Draw("P same");

    const std::string outfile(argv[2]);
    message(appname, "plot will be saved to `" + outfile + "'.", to_out);
    canvas->SaveAs(outfile.c_str());
    message(appname, "... gracefully done.", to_out);
}
