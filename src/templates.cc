/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#include "templates.h"
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include "Math/GSLIntegrator.h"
// #include "Math/SpecFuncMathMore.h"  // ROOT::Math::hyperg
#include "Math/WrappedTF1.h"
#include "TF1.h"
#include "cephes.h"
#include "histogram.h"
#include "info.h"
#include "utils.h"

namespace gg2aa {
void Template::set_template(const Info &info) {
    range_ = Range(info.xlow, info.xup);
    sqrt_s_ = info.rs;
    const auto f = std::make_unique<std::ifstream>(fname_);
    double x, y, z;
    maa_.reserve(1001);
    while (*f >> x >> y >> z) {
        if (range_.includes(x)) { maa_.push_back(y); }
    }
    f->close();
    maa_interval_ = range_.width() / (maa_.size() - 1);
}

double Template::f_maa(double x) const {
    if (!range_.includes(x)) { return 0.0; }
    const std::vector<double>::size_type pos =
        static_cast<int>(10.0 * (x - range_.low() + maa_interval_ / 2));
    if (pos >= maa_.size()) { return 0.0; }
    return maa_[pos];
}

// double simpson(std::function<double(double)> func, double xlow, double xup,
//                double delta) {
//     int n = (xup - xlow) / delta;
//     if (n % 2 == 1) {
//         --n;
//         xup -= delta;
//     }
//     double norm = func(xlow) + 4.0 * func(xup - delta) + func(xup);
//     n /= 2;
//     for (int i = 0; i != n; ++i) {
//         norm += 4.0 * func(xlow + (2 * i + 1) * delta) +
//                 2.0 * func(xlow + (2 * i + 2) * delta);
//     }
//     return norm * delta / 3.0;
// }

double Template::norm_sig() const {
    // return simpson(std::bind(&Template::f_maa, this, std::placeholders::_1),
    //                range_.low(), range_.up(), maa_interval_);
    TF1 f("f", [&](double *x, double *) { return f_maa(x[0]); }, range_.low(),
          range_.up(), 1);
    ROOT::Math::WrappedTF1 wf(f);
    ROOT::Math::GSLIntegrator ig(ROOT::Math::IntegrationOneDim::kADAPTIVE);
    ig.SetFunction(wf);
    ig.SetRelTolerance(0.001);
    return ig.Integral(range_.low(), range_.up());
}

// Eq.(2) in https://cds.cern.ch/record/2114853/files/ATLAS-CONF-2015-081.pdf
double fBG(const double x, const double s, const double p, const double b,
           const double a0, const double a1) {
#if defined(__APPLE__) && defined(__MACH__)
    return std::pow(1 - std::pow(x, p), b) *
           std::pow(x, a0 + a1 * std::log(x)) / s;
#elif defined(__linux__)
    return pow(1 - pow(x, p), b) * pow(x, a0 + a1 * log(x)) / s;
#endif
}

class FuncBG {
public:
    FuncBG() = delete;
    FuncBG(double s, double p, double b, double a0, double a1)
        : s_(s), p_(p), b_(b), a0_(a0), a1_(a1) {}
    ~FuncBG() {}

    double operator()(double *x, double *) const {
        return fBG(x[0], s_, p_, b_, a0_, a1_);
    }

private:
    const double s_, p_, b_, a0_, a1_;
};

double integralNormBG(const double x0, const double x1, const double p,
                      const double b, const double a0, const double a1) {
    FuncBG func_bg(1, p, b, a0, a1);
    TF1 f("f", func_bg, x0, x1, 0);
    ROOT::Math::WrappedTF1 wf1(f);
    // ROOT::Math::GSLIntegrator ig(ROOT::Math::IntegrationOneDim::kADAPTIVE);
    ROOT::Math::GSLIntegrator ig(ROOT::Math::IntegrationOneDim::kNONADAPTIVE);
    ig.SetFunction(wf1);
    ig.SetRelTolerance(0.001);
    return ig.Integral(x0, x1);
}

double fit_func_bg_nolog(const Template &t, const double x, const double p,
                         const double b, const double a0) {
    const double sqrt_s = t.sqrt_s_;
    const double x0 = t.range_.low() / sqrt_s, x1 = t.range_.up() / sqrt_s;
    const double a0_1 = a0 + 1;
    const double b1 = -b;
    const double b2 = a0_1 / p;
    const double b3 = b2 + 1;

    double scale = 1.0 / a0_1;
// use the hypergeometric function from Cephes.
#if defined(__APPLE__) && defined(__MACH__)
    scale *= std::pow(x1, a0_1) * hyp2f1(b1, b2, b3, std::pow(x1, p)) -
             std::pow(x0, a0_1) * hyp2f1(b1, b2, b3, std::pow(x0, p));
#elif defined(__linux__)
    scale *= pow(x1, a0_1) * hyp2f1(b1, b2, b3, pow(x1, p)) -
             pow(x0, a0_1) * hyp2f1(b1, b2, b3, pow(x0, p));
#endif
    return fBG(x, scale * sqrt_s, p, b, a0, 0);
}

double fit_func_bg1(const Template &t, const double x, const double s,
                    const double p, const double b, const double a0,
                    const double a1) {
    ignore(s);
    ignore(p);
    ignore(a1);
    return fit_func_bg_nolog(t, x, 1.0 / 3, b, a0);
}

double fit_func_bg2(const Template &t, const double x, const double s,
                    const double p, const double b, const double a0,
                    const double a1) {
    ignore(s);
    ignore(a1);
    return fit_func_bg_nolog(t, x, p, b, a0);
}

double fit_func_bg3(const Template &t, const double x, const double s,
                    const double p, const double b, const double a0,
                    const double a1) {
    ignore(s);
    ignore(a0);
    ignore(a1);
    return fit_func_bg_nolog(t, x, p, b, 0);
}

double fit_func_bg4(const Template &t, const double x, const double s,
                    const double p, const double b, const double a0,
                    const double a1) {
    ignore(s);
    ignore(p);
    ignore(a0);
    ignore(a1);
    const double sqrt_s = t.sqrt_s_;
    const double x0 = t.range_.low() / sqrt_s, x1 = t.range_.up() / sqrt_s;
    const double z0 = std::cbrt(x0), z1 = std::cbrt(x1);
    const double b1 = b + 1;
    const double b2 = b1 + 1;
    auto func = [](const double y, const double b) {
        return std::pow(1 - std::cbrt(y), b);
    };

    double scale = 3.0 / (b1 * b2 * (b2 + 1));
    scale *= func(x0, b1) * (2 + b1 * (2 + b2 * z0) * z0) -
             func(x1, b1) * (2 + b1 * (2 + b2 * z1) * z1);
    return func(x, b) / (scale * sqrt_s);
}

double fit_func_bg5(const Template &t, const double x, const double s,
                    const double p, const double b, const double a0,
                    const double a1) {
    ignore(s);
    ignore(p);
    const double sqrt_s = t.sqrt_s_;
    const double x0 = t.range_.low() / sqrt_s, x1 = t.range_.up() / sqrt_s;
    const double scale = integralNormBG(x0, x1, 1.0 / 3, b, a0, a1);
    return fBG(x, scale * sqrt_s, 1.0 / 3, b, a0, a1);
}

double fit_func_bg6(const Template &t, const double x, const double s,
                    const double p, const double b, const double a0,
                    const double a1) {
    ignore(t);
    ignore(p);
    return fBG(x, s, 1.0 / 3, b, a0, a1);
}

double fit_func_bg7(const Template &t, const double x, const double s,
                    const double p, const double b, const double a0,
                    const double a1) {
    ignore(s);
    ignore(p);
    ignore(a1);
    const double sqrt_s = t.sqrt_s_;
    const double x0 = t.range_.low() / sqrt_s, x1 = t.range_.up() / sqrt_s;
    const double scale = integralNormBG(x0, x1, 1.0 / 3, b, a0, 0);
    return fBG(x, scale * sqrt_s, 1.0 / 3, b, a0, 0);
}
}  // namespace gg2aa
