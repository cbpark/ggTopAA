/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#include "contours.h"
#include <memory>
#include <utility>
#include <vector>
#include "Math/Functor.h"
#include "Math/GSLSimAnMinimizer.h"
#include "TGraph2D.h"
#include "TH2D.h"
#include "fit.h"

namespace gg2aa {
std::shared_ptr<TGraph2D> MassWidthCont::mkGraph2D(
    const std::vector<FitResult> &fres) {
    auto g2 = std::make_shared<TGraph2D>();
    int npoint = 0;
    for (const auto &fr : fres) {
        if (fr.status() < 3) {
            g2->SetPoint(npoint, fr.mass(), fr.width(), fr.chi2());
            ++npoint;
        }
    }
    return g2;
}

void MassWidthCont::init_hist() {
    TH2D *hist = graph_->GetHistogram();
    hist->SetTitle("");
    hist->SetTitleOffset(0.9, "xy");
    hist->SetLineWidth(3);

    const double t_size = 1.4 * hist->GetLabelSize();
    hist->SetTitleSize(t_size, "xy");
    // hist->SetLabelSize(t_size, "xy");

    const int t_font = 132;
    hist->SetTitleFont(t_font, "xy");
    hist->SetLabelFont(t_font, "xy");

    auto xaxis = hist->GetXaxis();
    auto yaxis = hist->GetYaxis();

    xaxis->SetTitle("m_{t} (GeV)");
    xaxis->CenterTitle();
    xaxis->SetRangeUser(172, 174);
    xaxis->SetNdivisions(505);

    yaxis->SetTitle("#Gamma_{t} (GeV)");
    yaxis->CenterTitle();
    yaxis->SetRangeUser(0.5, 4);
    yaxis->SetNdivisions(504);
}

std::pair<std::array<double, 2>, double> minPoint(
    std::function<double(const double *)> func) {
    ROOT::Math::GSLSimAnMinimizer min;
    min.SetMaxFunctionCalls(1000000);
    min.SetMaxIterations(100000);
    min.SetTolerance(0.001);

    const ROOT::Math::Functor f(func, 2);
    min.SetFunction(f);

    const double step = 0.001;
    // min.SetLimitedVariable(0, "mass", 173.0, step, 172.0, 174.0);
    // min.SetLimitedVariable(1, "width", 1.5, step, 0.5, 4.0);
    min.SetVariable(0, "mass", 173.0, step);
    min.SetVariable(1, "width", 1.5, step);
    min.Minimize();
    // min.PrintResult();

    const double *point = min.X();
    return std::make_pair(std::array<double, 2>({{point[0], point[1]}}),
                          min.MinValue());
}
}  // namespace gg2aa
