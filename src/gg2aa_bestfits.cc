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
#include <tuple>
#include <utility>
// #include "TF1.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"
#include "TStyle.h"
#include "parsers.h"

using std::string;
using std::get;

struct HistBins {
    std::tuple<int, double, double> mass, width, kgg;
};

class Canvas {
public:
    Canvas() = delete;
    Canvas(const std::string &name)
        : canvas_(std::make_shared<TCanvas>(name.c_str())) {
        canvas_->SetTicks();
    }
    ~Canvas() {}

    void write_hist(std::unique_ptr<TH1D> hist) {
        canvas_->cd();
        hist->Draw();
        canvas_->Update();
        canvas_->Write();
    }

private:
    std::shared_ptr<TCanvas> canvas_;
};

int main(int argc, char *argv[]) {
    const string appname("gg2aa_bestfits");
    if (argc != 4) {
        string usage = "Usage: " + appname + " input output sqrt_s\n\n";
        usage += "    input  - input data file\n";
        usage += "    output - output file in the root format\n";
        usage += "    sqrt_s - center-of-mass energy (13, 100)\n\n";
        usage += "    ex) " + appname + " bestfit.dat output.root 13\n";
        return howToUse(usage);
    }

    const int sqrt_s = std::atoi(argv[3]);
    if (sqrt_s != 13 && sqrt_s != 100) {
        return errMsg(appname, "sqrt_s must be 13 or 100.");
    }

    auto infile = std::make_unique<std::ifstream>(argv[1]);
    if (!infile->good()) { return failedToRead(appname, argv[1]); }
    const auto to_out = &std::cout;  // information will be displayed in screen.

    auto bestfits = gg2aa::parseBestFitPoints(std::move(infile));
    message(appname, "`" + std::string(argv[1]) + "' has been parsed.", to_out);

    // Set bins of hitograms.
    HistBins bins;
    if (sqrt_s == 100) {
        bins.mass = std::make_tuple(20, 172, 174);
    } else {
        bins.mass = std::make_tuple(20, 170, 180);
    }
    bins.width = std::make_tuple(16, 0, 4);
    bins.kgg = std::make_tuple(100, 0, 1);

    gStyle->SetOptStat(1110);
    gStyle->SetOptFit(1);

    // Open a canvas.
    auto canvas_mass = Canvas("mass");
    auto canvas_width = Canvas("width");
    auto canvas_kgg = Canvas("kgg");

    const int t_font = 132;
    const int line_width = 3;

    // Create and fill histograms.
    auto hist_mass = std::make_unique<TH1D>(
        "mass", "", get<0>(bins.mass), get<1>(bins.mass), get<2>(bins.mass));
    hist_mass->GetXaxis()->SetTitle("m_{t} (GeV)");
    hist_mass->GetXaxis()->CenterTitle();
    hist_mass->SetTitleFont(t_font, "xy");
    hist_mass->SetLabelFont(t_font, "xy");
    hist_mass->SetTitleSize(1.4 * hist_mass->GetLabelSize(), "xy");
    hist_mass->SetLineWidth(line_width);
    hist_mass->SetNdivisions(505);

    auto hist_width =
        std::make_unique<TH1D>("width", "", get<0>(bins.width),
                               get<1>(bins.width), get<2>(bins.width));
    hist_width->GetXaxis()->SetTitle("#Gamma_{t} (GeV)");
    hist_width->GetXaxis()->CenterTitle();
    hist_width->SetTitleFont(t_font, "xy");
    hist_width->SetLabelFont(t_font, "xy");
    hist_width->SetTitleSize(1.4 * hist_width->GetLabelSize(), "xy");
    hist_width->SetLineWidth(line_width);

    auto hist_kgg = std::make_unique<TH1D>("kgg", "", get<0>(bins.kgg),
                                           get<1>(bins.kgg), get<2>(bins.kgg));
    hist_kgg->GetXaxis()->SetTitle("#kappa");
    hist_kgg->GetXaxis()->CenterTitle();
    hist_kgg->SetTitleFont(t_font, "xy");
    hist_kgg->SetLabelFont(t_font, "xy");
    hist_kgg->SetTitleSize(1.4 * hist_kgg->GetLabelSize(), "xy");
    hist_kgg->SetTitleFont(t_font);
    hist_kgg->SetLineWidth(line_width);

    for (const auto &p : bestfits) {
        hist_mass->Fill(p.mass);
        hist_width->Fill(p.width);
        hist_kgg->Fill(p.kgg);
    }

    // Fit histogram: mass.
    hist_mass->Fit("gaus");
    // Fit histogram: width.
    // auto cb_func = std::make_shared<TF1>("f1", "crystalball", 0, 4);
    // cb_func->SetParameters(1, 0, 1, 2, 0.5);
    // hist_width->Fit(cb_func.get());
    // hist_width->Fit("landau");
    hist_width->Fit("gaus", "", "", 0.5, 2.0);
    // Fit histogram: k_{gg}.
    hist_kgg->Fit("gaus");

    const string outfile_name(argv[2]);
    message(appname, "histograms will be saved to `" + outfile_name + "'.",
            to_out);
    auto outfile = std::make_unique<TFile>(outfile_name.c_str(), "RECREATE");

    canvas_mass.write_hist(std::move(hist_mass));
    canvas_width.write_hist(std::move(hist_width));
    canvas_kgg.write_hist(std::move(hist_kgg));

    outfile->Close();
    message(appname, "... gracefully done.", to_out);
}
