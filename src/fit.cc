#include "fit.h"
#include "info.h"
#include "templates.h"

namespace gg2aa {
double FitFunction::operator()(double *x, double *p) const {
    const double fgg = template_.f_maa(x[0]) / template_.norm();
    double f = nevent_ / nbins_ * template_.range().width();
    f *= (1.0 - p[2]) / sqrt_s_ * fATL(template_, x[0] / sqrt_s_, p[0], p[1]) +
         p[2] * fgg;
    return f;
}

void Fit::set_parameters(const Info &info) {
    pfnc_->SetParNames("a1", "a2", "b");
    pfnc_->SetParLimits(0, -1.0e4, 1.0e4);
    pfnc_->SetParLimits(1, -1.0e4, 1.0e4);
    pfnc_->SetParLimits(2, 0.0, 1.0);
    pfnc_->SetParameters(info.a1_in, info.a2_in, info.b_in);
    pfnc_->FixParameter(1, 0.0);
}
}  // namespace gg2aa
