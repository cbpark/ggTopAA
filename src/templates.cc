#include "templates.h"
#include <cmath>
#include <fstream>
#include <functional>
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

double poly_cbrt(double x, double a1, double a2) {
    return std::pow((1.0 - std::cbrt(x)), a1) * std::pow(x, a2);
}

double fATL(const Template &t, double x, double a1, double a2) {
    const double xmax = t.range_.up() / t.sqrt_s_,
                 xmin = t.range_.low() / t.sqrt_s_;
    double sATL = 0.0;

    if (std::fabs(1.0 + a2) < 1.0e-3) {
        const int nbins = 4 * t.range_.width();
        const double delta = (xmax - xmin) / nbins;
        for (int i = 0; i <= nbins; ++i) {
            sATL += poly_cbrt(xmin + delta * i, a1, a2);
        }
        sATL *= delta;
    } else {
        double a = -a1, b = 3.0 * (1.0 + a2), c = 4.0 + 3.0 * a2;
        const double cbrt_xmax = std::cbrt(xmax), cbrt_xmin = std::cbrt(xmin);
        double pf1 = 1.0, pf2 = 1.0;
        if (a < -10.0 || b < -10.0) {
            pf1 = std::pow(1.0 - cbrt_xmax, c - b - a);
            pf2 = std::pow(1.0 - cbrt_xmin, c - b - a);
            a = c - a;
            b = c - b;
        }
        const double hyp1 = pf1 * ROOT::Math::hyperg(a, b, c, cbrt_xmax);
        const double hyp2 = pf2 * ROOT::Math::hyperg(a, b, c, cbrt_xmin);

        sATL = (std::pow(xmax, 1.0 + a2) * hyp1 -
                std::pow(xmin, 1.0 + a2) * hyp2) /
               (1.0 + a2);
    }
    return poly_cbrt(x, a1, a2) / sATL;
}
}  // namespace gg2aa
