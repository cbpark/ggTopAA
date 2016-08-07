#include "fit.h"
#include "templates.h"

namespace gg2aa {
double FitFunction::operator()(double *x, double *p) {
    const double fgg = template_.f_maa(x[0]) / template_.norm();
    double f = nevent_ / nbins_ * template_.range().width();
    f *= (1.0 - p[2]) / sqrt_s_ * fATL(template_, x[0] / sqrt_s_, p[0], p[1]) +
         p[2] * fgg;
    return f;
}
}  // namespace gg2aa
