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
#include <iostream>
#include <memory>
#include "Math/MinimizerOptions.h"
#include "TFitResult.h"
#include "TH1D.h"
#include "info.h"
#include "templates.h"
#include "utils.h"

using std::shared_ptr;

const double EPS = 1.0e-12;

namespace gg2aa {
double FitFunction::operator()(double *x, double *p) const {
    if (std::fabs(p[0]) < EPS || std::fabs(p[1]) < EPS ||
        std::fabs(p[3] + 1) < EPS) {  // if s -> or p -> 0 or a0 + 1 -> 0
        TF1::RejectPoint();
        return 0;
    }

    const double fgg = template_.f_maa(x[0]) / norm_sig_;
    double f = info_.nev() / info_.num_bins() * template_.range().width();
    // p: s, p, b, a0, a1, kgg
    f *= (1.0 - p[5]) *
             func_(template_, x[0] / info_.rs, p[0], p[1], p[2], p[3], p[4]) +
         p[5] * fgg;
    return f;
}

std::istream &operator>>(std::istream &is, FitResult &fres) {
    is >> fres.mass_ >> fres.width_ >> fres.chi2_ >> fres.chi2_ndf_;
    const int par_size(fres.par_.size());
    for (int i = 0; i != par_size; ++i) { is >> fres.par_[i]; }
    is >> fres.status_;
    return is;
}

std::ostream &operator<<(std::ostream &os, const FitResult &fres) {
    os << std::fixed;
    os << std::setw(9) << std::setprecision(2) << fres.mass_;
    os << std::setw(8) << std::setprecision(2) << fres.width_;
    os << std::setw(12) << std::setprecision(4) << fres.chi2_;
    os << std::setw(11) << std::setprecision(4) << fres.chi2_ndf_;
    for (const auto p : fres.par_) {
        os << std::setw(12) << std::setprecision(5) << p;
    }
    os << std::setw(6) << fres.status_;
    return os;
}

void Fit::init_parameters(const Info &info) {
    pfnc_->SetParNames("s", "p", "b", "a0", "a1", "kgg");
    pfnc_->SetParameters(info.s_in, info.p_in, info.b_in, info.a0_in,
                         info.a1_in, info.kgg_in);
}

void Fit::do_fit(shared_ptr<TH1D> hist, shared_ptr<FitResult> result) {
    ROOT::Math::MinimizerOptions opt;
    opt.SetDefaultMinimizer("Minuit2", "Minimize");
    opt.SetMaxFunctionCalls(1000000);
    opt.SetMaxIterations(100000);
    opt.SetTolerance(0.001);

    // - "I": use integral of function in bin instead of value at bin center.
    // - "N": do not store the graphics function, do not draw.
    // - "S": the result of the fit is returned in the TFitResultPtr.
    auto r = hist->Fit(pfnc_.get(), "LNS", "", 330, 360);

    const int npar(r->NPar());
    FitParameters par;
    for (int i = 0; i != npar; ++i) { par[i] = r->Parameter(i); }

    result->set_result(par, r->Chi2(), r->Ndf(), r->Status());
}

void BestFitPoint::show(std::ostream *out) const {
    *out << "--- Best-fit point ---\n";
    *out << "  " << get_string("mass", mass) << ", "
         << get_string("width", width) << ", " << get_string("kgg", kgg) << ", "
         << get_string("chi2", chi2) << '\n';
}

std::istream &operator>>(std::istream &is, BestFitPoint &pos) {
    is >> pos.mass >> pos.width >> pos.kgg >> pos.chi2;
    return is;
}

std::ostream &operator<<(std::ostream &os, const BestFitPoint &pos) {
    os << std::fixed;
    os << std::setw(9) << std::setprecision(2) << pos.mass;
    os << std::setw(8) << std::setprecision(2) << pos.width;
    os << std::setw(12) << std::setprecision(5) << pos.kgg;
    os << std::setw(12) << std::setprecision(4) << pos.chi2;
    return os;
}

Fit mkFit(const Template &t, const Info &info, const int fit_choice) {
    switch (fit_choice) {
    case 1:
        return fit1(t, info);
    case 2:
        return fit2(t, info);
    case 3:
        return fit3(t, info);
    case 4:
        return fit4(t, info);
    case 5:
        return fit5(t, info);
    default:
        return fit6(t, info);
    }
}

Fit fit1(const Template &t, const Info &info) {
    const FitFunction ffnc(t, info, fit_func_bg1);
    auto fit = Fit(ffnc);
    fit.pfnc()->FixParameter(0, 1);        // s
    fit.pfnc()->FixParameter(1, 1.0 / 3);  // p
    fit.pfnc()->SetParLimits(2, 0, 1000);  // b
#if defined(__APPLE__) && defined(__MACH__)
    fit.pfnc()->SetParLimits(3, -10, 10);  // a0
#elif defined(__linux__)
    fit.pfnc()->SetParLimits(3, -10, 0);  // a0
#endif
    fit.pfnc()->FixParameter(4, 0);     // a1
    fit.pfnc()->SetParLimits(5, 0, 1);  // kgg
    return fit;
}

Fit fit2(const Template &t, const Info &info) {
    const FitFunction ffnc(t, info, fit_func_bg2);
    auto fit = Fit(ffnc);
    fit.pfnc()->FixParameter(0, 1);        // s
    fit.pfnc()->SetParLimits(1, 0, 10);    // p
    fit.pfnc()->SetParLimits(2, 0, 1000);  // b
#if defined(__APPLE__) && defined(__MACH__)
    fit.pfnc()->SetParLimits(3, -10, 10);  // a0
#elif defined(__linux__)
    fit.pfnc()->SetParLimits(3, -10, 0);  // a0
#endif
    fit.pfnc()->FixParameter(4, 0);     // a1
    fit.pfnc()->SetParLimits(5, 0, 1);  // kgg
    return fit;
}

Fit fit3(const Template &t, const Info &info) {
    const FitFunction ffnc(t, info, fit_func_bg3);
    auto fit = Fit(ffnc);
    fit.pfnc()->FixParameter(0, 1);        // s
    fit.pfnc()->SetParLimits(1, 0, 10);    // p
    fit.pfnc()->SetParLimits(2, 0, 1000);  // b
    fit.pfnc()->FixParameter(3, 0);        // a0
    fit.pfnc()->FixParameter(4, 0);        // a1
    fit.pfnc()->SetParLimits(5, 0, 1);     // kgg
    return fit;
}

Fit fit4(const Template &t, const Info &info) {
    const FitFunction ffnc(t, info, fit_func_bg4);
    auto fit = Fit(ffnc);
    fit.pfnc()->FixParameter(0, 1);        // s
    fit.pfnc()->FixParameter(1, 1.0 / 3);  // p
    fit.pfnc()->SetParLimits(2, 0, 1000);  // b
    fit.pfnc()->FixParameter(3, 0);        // a0
    fit.pfnc()->FixParameter(4, 0);        // a1
    fit.pfnc()->SetParLimits(5, 0, 1);     // kgg
    return fit;
}

Fit fit5(const Template &t, const Info &info) {
    const FitFunction ffnc(t, info, fit_func_bg5);
    auto fit = Fit(ffnc);
    fit.pfnc()->FixParameter(0, 1);        // s
    fit.pfnc()->FixParameter(1, 1.0 / 3);  // p
    fit.pfnc()->SetParLimits(2, 0, 1000);  // b
    fit.pfnc()->SetParLimits(3, -1, 1);    // a0
    fit.pfnc()->SetParLimits(4, -1, 1);    // a1
    fit.pfnc()->SetParLimits(5, 0, 1);     // kgg
    return fit;
}

Fit fit6(const Template &t, const Info &info) {
    const FitFunction ffnc(t, info, fit_func_bg6);
    auto fit = Fit(ffnc);
    fit.pfnc()->SetParLimits(0, 0, 0.01);  // s
    fit.pfnc()->FixParameter(1, 1.0 / 3);  // p
    fit.pfnc()->SetParLimits(2, 0, 1000);  // b
    fit.pfnc()->SetParLimits(3, -1, 1);    // a0
    fit.pfnc()->SetParLimits(4, -1, 1);    // a1
    fit.pfnc()->SetParLimits(5, 0, 1);     // kgg
    return fit;
}
}  // namespace gg2aa
