/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#include "gg2aa.h"
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include "TFile.h"
#include "TH1D.h"
#include "parsers.h"

using std::string;

int main(int argc, char *argv[]) {
    const string appname("gg2aa_bestfits");
    if (argc != 3) {
        string usage = "Usage: " + appname + " input output\n\n";
        usage += "    input  - input data file\n";
        usage += "    output - output file in the root format\n\n";
        usage += "    ex) " + appname + " bestfit.dat output.root\n";
        return howToUse(usage);
    }

    auto infile = std::make_unique<std::ifstream>(argv[1]);
    if (!infile->good()) { return failedToRead(appname, argv[1]); }
    const auto to_out = &std::cout;  // information will be displayed in screen.

    auto bestfits = gg2aa::parseBestFitPoints(std::move(infile));
    message(appname, "`" + std::string(argv[1]) + "' has been parsed.", to_out);

    auto hist_mass = std::make_unique<TH1D>("mass", "", 20, 172.0, 174.0);
    auto hist_width = std::make_unique<TH1D>("width", "", 10, 0.0, 4.0);
    auto hist_kgg = std::make_unique<TH1D>("kgg", "", 40, 0.0, 1.0);
    for (const auto &p : bestfits) {
        hist_mass->Fill(p.mass);
        hist_width->Fill(p.width);
        hist_kgg->Fill(p.kgg);
    }

    const string outfile_name(argv[2]);
    message(appname, "histograms will be saved to `" + outfile_name + "'.",
            to_out);
    auto outfile = std::make_unique<TFile>(outfile_name.c_str(), "RECREATE");
    hist_mass->Write();
    hist_width->Write();
    hist_kgg->Write();

    outfile->Close();
    message(appname, "... gracefully done.", to_out);
}
