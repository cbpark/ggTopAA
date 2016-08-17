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
#include "Math/SpecFuncMathMore.h"
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

double Template::norm() const {
    return simpson(std::bind(&Template::f_maa, this, std::placeholders::_1),
                   range_.low(), range_.up(), maa_interval_);
}

double hyp2f1(double a, double b, double c, const double z) {
    double p = 1.0;
    if (a < -10.0 || b < -10.0) {
        p = std::pow(1 - z, c - a - b);
        a = c - a;
        b = c - b;
    }
    return p * ROOT::Math::hyperg(a, b, c, z);
}

double func_maa3(const Template &t, const double x, const double a,
                 const double p) {
    const double x0 = t.range_.low() / t.sqrt_s_,
                 x1 = t.range_.up() / t.sqrt_s_;
    const double xx1 = -a, xx2 = 1.0 / p, xx3 = 1.0 + xx2;
    const double s = x1 * hyp2f1(xx1, xx2, xx3, std::pow(x1, p)) -
                     x0 * hyp2f1(xx1, xx2, xx3, std::pow(x0, p));
    return std::pow(1 - std::pow(x, p), a) / s;
}

double func_maa4(const Template &t, const double x, const double a) {
    const double x0 = t.range_.low() / t.sqrt_s_,
                 x1 = t.range_.up() / t.sqrt_s_;
    const double z0 = std::cbrt(x0), z1 = std::cbrt(x1);
    const double a1 = 1.0 + a, a2 = 2.0 + a;
    auto func = [](const double xx, const double aa) {
        return std::pow(1 - std::cbrt(xx), aa);
    };

    double s = 3.0 / (a1 * a2 * (3.0 + a));
    s *= func(x0, a1) * (2.0 + a1 * (2.0 + a2 * z0) * z0) -
         func(x1, a1) * (2.0 + a1 * (2.0 + a2 * z1) * z1);
    return func(x, a) / s;
}
}  // namespace gg2aa
