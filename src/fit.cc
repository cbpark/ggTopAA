/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#include "fit.h"
#include <cmath>
#include <iomanip>
#include <memory>
#include <ostream>
#include <vector>
#include "Math/MinimizerOptions.h"
#include "TFitResult.h"
#include "TH1D.h"
#include "info.h"
#include "templates.h"

const double EPS = 1.0e-9;

namespace gg2aa {
double FitFunction::operator()(double *x, double *p) const {
    if (fabs(p[1] + 1.0) < EPS || fabs(p[2]) < EPS) {  // if a2 + 1 or p -> 0
        TF1::RejectPoint();
        return 0;
    }

    const double fgg = template_.f_maa(x[0]) / norm_sig_;
    const double sqrt_s = info_.rs;
    double f = info_.nev() / info_.num_bins() * template_.range().width();
    f *= (1.0 - p[3]) / sqrt_s *
             func_(template_, x[0] / sqrt_s, p[0], p[1], p[2]) +
         p[3] * fgg;
    return f;
}

void FitResult::write(std::shared_ptr<std::ostream> os) const {
    *os << std::fixed;
    *os << std::setw(9) << std::setprecision(2) << mass_;
    *os << std::setw(8) << std::setprecision(2) << width_;
    *os << std::setw(12) << std::setprecision(4) << chi2_;
    *os << std::setw(11) << std::setprecision(4) << chi2_ndf_;
    for (const auto p : par_) {
        *os << std::setw(11) << std::setprecision(4) << p;
    }
    *os << '\n';
}

void Fit::init_parameters(const Info &info) {
    pfnc_->SetParNames("a1", "a2", "p", "kgg");
    pfnc_->SetParameters(info.a1_in, info.a2_in, info.p_in, info.kgg_in);
}

void Fit::do_fit(std::shared_ptr<TH1D> hist,
                 std::shared_ptr<FitResult> result) {
    ROOT::Math::MinimizerOptions opt;
    opt.SetDefaultMinimizer("Minuit2", "Minimize");
    opt.SetMaxFunctionCalls(1000000);
    opt.SetMaxIterations(100000);
    opt.SetTolerance(0.001);

    // - "I": use integral of function in bin instead of value at bin center.
    // - "N": do not store the graphics function, do not draw.
    // - "S": the result of the fit is returned in the TFitResultPtr.
    auto r = hist->Fit(pfnc_.get(), "INS");

    int npar = r->NPar();
    std::vector<double> par;
    for (int i = 0; i != npar; ++i) { par.push_back(r->Parameter(i)); }
    result->set_result(par, r->Chi2(), r->Ndf());
}

Fit mkFit(const Template &t, const Info &info, const int fit_choice) {
    switch (fit_choice) {
    case 1:
        return fit1(t, info);
    case 2:
        return fit2(t, info);
    case 3:
        return fit3(t, info);
    default:
        return fit4(t, info);
    }
}

Fit fit1(const Template &t, const Info &info) {
    const FitFunction ffnc(t, info, norm_bg1);
    auto fit = Fit(ffnc);
    fit.pfnc()->SetParLimits(0, 0, 1000);  // a1
    fit.pfnc()->SetParLimits(1, -10, 0);   // a2
    fit.pfnc()->FixParameter(2, 1.0 / 3);  // p
    fit.pfnc()->SetParLimits(3, 0, 1);     // kgg
    return fit;
}

Fit fit2(const Template &t, const Info &info) {
    const FitFunction ffnc(t, info, norm_bg2);
    auto fit = Fit(ffnc);
    fit.pfnc()->SetParLimits(0, 0, 1000);  // a1
    fit.pfnc()->SetParLimits(1, -10, 0);   // a2
    fit.pfnc()->SetParLimits(2, 0, 10);    // p
    fit.pfnc()->SetParLimits(3, 0, 1);     // kgg
    return fit;
}

Fit fit3(const Template &t, const Info &info) {
    const FitFunction ffnc(t, info, norm_bg3);
    auto fit = Fit(ffnc);
    fit.pfnc()->SetParLimits(0, 0, 1000);  // a1
    fit.pfnc()->FixParameter(1, 0);        // a2
    fit.pfnc()->SetParLimits(2, 0, 10);    // p
    fit.pfnc()->SetParLimits(3, 0, 1);     // kgg
    return fit;
}

Fit fit4(const Template &t, const Info &info) {
    const FitFunction ffnc(t, info, norm_bg4);
    auto fit = Fit(ffnc);
    fit.pfnc()->SetParLimits(0, 0, 1000);  // a1
    fit.pfnc()->FixParameter(1, 0);        // a2
    fit.pfnc()->FixParameter(2, 1.0 / 3);  // p
    fit.pfnc()->SetParLimits(3, 0, 1);     // kgg
    return fit;
}
}  // namespace gg2aa
