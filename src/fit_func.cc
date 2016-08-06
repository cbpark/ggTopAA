#include "fit_func.h"
#include "templates.h"

namespace gg2aa {
double FitFunction::operator()(double *x, double *p) {
    const double rs  = template_.sqrt_s();
    const double fgg = template_.f_maa(x[0]) / template_.norm();
    double f         = nevent_ / num_bins_ * (template_.width());
    f *= (1.0 - p[2]) / rs *
             fATL(template_, 4.0 * (template_.width()), x[0] / rs, p[0], p[1]) +
         p[2] * fgg;
    return f;
}
}  // namespace gg2aa
