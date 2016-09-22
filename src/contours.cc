/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#include "contours.h"
#include <functional>
#include <memory>
#include <utility>
#include <vector>
#include "Math/Functor.h"
#include "Math/GSLSimAnMinimizer.h"
#include "TAxis.h"
#include "TGraph2D.h"
#include "TH2D.h"
#include "fit.h"

using std::shared_ptr;
using std::vector;

namespace gg2aa {
shared_ptr<TGraph> graphMinPoint(const Contour &cont) {
    auto gr = std::make_shared<TGraph>(1);
    const std::array<double, 2> mpoint = cont.min_point();
    gr->SetPoint(0, mpoint[0], mpoint[1]);
    gr->SetMarkerStyle(20);
    return gr;
}

void set_cont_levels(const double oneSigma, const double twoSigma,
                     Contour *cont) {
    const double min_value = cont->min_value();
    const double contours[2] = {min_value + oneSigma, min_value + twoSigma};
    cont->graph()->GetHistogram()->SetContour(2, contours);
}

void Contour::set_min_point(const double x, const double y, const double step,
                            std::function<double(const double *)> func) {
    ROOT::Math::GSLSimAnMinimizer min;
    min.SetMaxFunctionCalls(1000000);
    min.SetMaxIterations(100000);
    min.SetTolerance(0.001);

    const ROOT::Math::Functor f(func, 2);
    min.SetFunction(f);

    min.SetVariable(0, "x", x, step);
    min.SetVariable(1, "y", y, step);
    min.Minimize();
    // min.PrintResult();

    const double *point = min.X();
    min_point_ = {{point[0], point[1]}};
    min_value_ = min.MinValue();
}

void setAxisInfo(const ContAxisInfo &axinfo, TAxis *axis) {
    axis->SetTitle(axinfo.title.c_str());
    axis->CenterTitle();
    axis->SetRangeUser(axinfo.range[0], axinfo.range[1]);
    axis->SetNdivisions(axinfo.ndiv);
}

void Contour::set_hist(const ContAxisInfo &x_axis, const ContAxisInfo &y_axis,
                       shared_ptr<TGraph2D> graph) {
    TH2D *hist = graph->GetHistogram();
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
    setAxisInfo(x_axis, xaxis);

    auto yaxis = hist->GetYaxis();
    setAxisInfo(y_axis, yaxis);
}

shared_ptr<TGraph2D> MassWidthCont::mkGraph2D(const vector<FitResult> &fres) {
    auto g2 = std::make_shared<TGraph2D>();
    int npoint = 0;
    for (const auto &fr : fres) {
        if (fr.status() < 3) {  // status > 2 is invalid result.
            g2->SetPoint(npoint, fr.mass(), fr.width(), fr.chi2());
            ++npoint;
        }
    }
    return g2;
}

std::pair<double, double> chi2Interval12_ndf(const int ndf) {
    switch (ndf) {
    case 2:
        return std::make_pair(2.29575, 6.18007);
    case 3:
        return std::make_pair(3.52674, 8.02488);
    case 4:
        return std::make_pair(4.71947, 9.71563);
    case 5:
        return std::make_pair(5.8876, 11.3139);
    case 6:
        return std::make_pair(7.0384, 12.8488);
    case 7:
        return std::make_pair(8.17624, 14.3371);
    case 8:
        return std::make_pair(9.30391, 15.7891);
    case 9:
        return std::make_pair(10.4234, 17.2118);
    case 10:
        return std::make_pair(11.536, 18.6103);
    default:
        return std::make_pair(1, 4);
    }
}

std::pair<double, double> chi2Interval12(const int fit_choice) {
    if (fit_choice == 1 || fit_choice == 3) {
        return chi2Interval12_ndf(5);
    } else if (fit_choice == 2 || fit_choice == 5 || fit_choice == 6) {
        return chi2Interval12_ndf(6);
    } else if (fit_choice == 4) {
        return chi2Interval12_ndf(4);
    } else {
        return chi2Interval12_ndf(1);
    }
}
}  // namespace gg2aa
