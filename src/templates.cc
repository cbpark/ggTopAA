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
#include "cephes_hyp2f1.h"
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

double simpson(std::function<double(double)> func, double xlow, double xup,
               double delta) {
    int n = (xup - xlow) / delta;
    if (n % 2 == 1) {
        --n;
        xup -= delta;
    }
    n /= 2;
    double norm = func(xlow) + 4.0 * func(xup - delta) + func(xup);
    for (int i = 0; i != n; ++i) {
        norm += 4.0 * func(xlow + (2 * i + 1) * delta) +
                2.0 * func(xlow + (2 * i + 2) * delta);
    }
    return norm * delta / 3.0;
}

double Template::norm_sig() const {
    return simpson(std::bind(&Template::f_maa, this, std::placeholders::_1),
                   range_.low(), range_.up(), maa_interval_);
}

double fBG(const double x, const double a1, const double a2,
               const double p) {
    return std::pow(1 - std::pow(x, p), a1) * std::pow(x, a2);
}

class FuncBG {
public:
    FuncBG() = delete;
    FuncBG(double a1, double a2, double p) : a1_(a1), a2_(a2), p_(p) {}
    ~FuncBG() {}

    double operator()(double *x, double *par) {
        ignore(par);
        return fBG(x[0], a1_, a2_, p_);
    }

private:
    const double a1_, a2_, p_;
};

double integralNormBG(const double x0, const double x1, const double a1,
                      const double a2, const double p) {
    FuncBG func_bg(a1, a2, p);
    TF1 f("f", func_bg, x0, x1, 0);
    ROOT::Math::WrappedTF1 wf1(f);
    // ROOT::Math::GSLIntegrator ig(ROOT::Math::IntegrationOneDim::kADAPTIVE);
    ROOT::Math::GSLIntegrator ig(ROOT::Math::IntegrationOneDim::kNONADAPTIVE);
    ig.SetFunction(wf1);
    ig.SetRelTolerance(0.001);
    return ig.Integral(x0, x1);
}

double norm_bg(const Template &t, const double x, const double a1,
               const double a2, const double p) {
    const double x0 = t.range_.low() / t.sqrt_s_,
                 x1 = t.range_.up() / t.sqrt_s_;
    const double a2_1 = a2 + 1;
    const double b1 = -a1, b2 = a2_1 / p, b3 = 1 + b2;

    double s = 1.0 / a2_1;
    // use the hypergeometric function from ROOT with GSL.
    // s *= std::pow(x1, a2_1) * ROOT::Math::hyperg(b1, b2, b3, std::pow(x1, p))
    // -
    //      std::pow(x0, a2_1) * ROOT::Math::hyperg(b1, b2, b3, std::pow(x0,
    //      p));
    // use the hypergeometric function from Cephes.
    s *= std::pow(x1, a2_1) * hyp2f1(b1, b2, b3, std::pow(x1, p)) -
         std::pow(x0, a2_1) * hyp2f1(b1, b2, b3, std::pow(x0, p));

    return fBG(x, a1, a2, p) / s;
}

double norm_bg1(const Template &t, const double x, const double a1,
                const double a2, const double p) {
    ignore(p);
    return norm_bg(t, x, a1, a2, 1.0 / 3);
}

double norm_bg2(const Template &t, const double x, const double a1,
                const double a2, const double p) {
    return norm_bg(t, x, a1, a2, p);
}

double norm_bg3(const Template &t, const double x, const double a1,
                const double a2, const double p) {
    ignore(a2);
    return norm_bg(t, x, a1, 0, p);
}

double norm_bg4(const Template &t, const double x, const double a1,
                const double a2, const double p) {
    ignore(a2);
    ignore(p);
    const double x0 = t.range_.low() / t.sqrt_s_,
                 x1 = t.range_.up() / t.sqrt_s_;
    const double z0 = std::cbrt(x0), z1 = std::cbrt(x1);
    const double b1 = 1 + a1, b2 = 2 + a1;
    auto func = [](const double y, const double b) {
        return std::pow(1 - std::cbrt(y), b);
    };

    double s = 3.0 / (b1 * b2 * (3 + a1));
    s *= func(x0, b1) * (2 + b1 * (2 + b2 * z0) * z0) -
         func(x1, b1) * (2 + b1 * (2 + b2 * z1) * z1);
    return func(x, a1) / s;
}
}  // namespace gg2aa
