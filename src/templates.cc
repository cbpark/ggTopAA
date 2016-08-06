#include "templates.h"
#include <cmath>
#include <fstream>
#include <functional>
#include <memory>
#include "Math/SpecFuncMathMore.h"

namespace gg2aa {
void Template::set_template(double xlow, double xup, double sqrt_s) {
    xlow_   = xlow;
    xup_    = xup;
    sqrt_s_ = sqrt_s;
    double x, y, z;
    std::unique_ptr<std::ifstream> f(new std::ifstream(fname_));
    while (*f >> x >> y >> z) {
        if (x >= xlow_ && x <= xup_) { m_aa_.emplace(x, y); }
    }
    maa_interval_ = (xup - xlow) / (m_aa_.size() - 1);
}

double Template::f_maa(double x) const {
    if (x < xlow_ || x > xup_) { return 0.0; }
    return m_aa_.lower_bound(x)->second;
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
                   xlow_, xup_, maa_interval_);
}

double poly_cbrt(double x, double a1, double a2) {
    return std::pow((1.0 - std::cbrt(x)), a1) * std::pow(x, a2);
}

double fATL(const Template &t, int nbins, double x, double a1, double a2) {
    const double xmax = t.xup_ / t.sqrt_s_, xmin = t.xlow_ / t.sqrt_s_;

    double sATL = 0.0;
    if (std::fabs(1.0 + a2) < 1.0e-3) {
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
            a   = c - a;
            b   = c - b;
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
