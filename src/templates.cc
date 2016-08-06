#include "templates.h"
#include <fstream>
#include <functional>
#include <memory>
#include "histogram.h"
#include "inputinfo.h"
#include "utils.h"

namespace gg2aa {
void Template::set_template(const InputInfo &info) {
    range_  = Range(info.xlow, info.xup);
    sqrt_s_ = info.rs;

    std::unique_ptr<std::ifstream> f(new std::ifstream(fname_));
    double x, y, z;
    while (*f >> x >> y >> z) {
        if (range_.includes(x)) { maa_.emplace(x, y); }
    }
    maa_interval_ = range_.width() / (maa_.size() - 1);
}

double Template::f_maa(double x) const {
    if (!range_.includes(x)) { return 0.0; }
    return maa_.lower_bound(x)->second;
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
}  // namespace gg2aa
