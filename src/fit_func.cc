#include "fit_func.h"
#include "histograms.h"

namespace gg2aa {
double FitFunction::operator() (double *x, double *p) {
    const double rs = hist_.sqrt_s();
    const double fgg = hist_.f_maa(x[0]) / hist_.norm_signal();
    double f = hist_.nevent() / hist_.bg_num_bins() * hist_.hist_width();
    f *= (1.0 - p[2]) / rs * fATL(hist_, x[0] / rs, p[0], p[1]) + p[2] * fgg;
    return f;
}
}  // namespace gg2aa
