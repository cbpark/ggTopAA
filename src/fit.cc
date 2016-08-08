#include "fit.h"
#include <iostream>
#include "info.h"
#include "templates.h"

namespace gg2aa {
double FitFunction::operator()(double *x, double *p) {
    // std::cout << "FitFunction::operator() x = " << x[0] << ", p = " << p[0]
    // << '\n';
    // std::cout << "FitFunction::operator: norm = " << template_.norm() <<
    // '\n';
    const double fgg = template_.f_maa(x[0]) / template_.norm();
    double f = nevent_ / nbins_ * template_.range().width();
    f *= (1.0 - p[2]) / sqrt_s_ * fATL(template_, x[0] / sqrt_s_, p[0], p[1]) +
         p[2] * fgg;
    // std::cout << "FitFunction::operator: x[0] = " << x[0] << ", p[0] = " << p[0]
    //           << ", p[1] = " << p[1] << ", p[2] = " << p[2] << ", f = " << f
    //           << '\n';
    return f;
}

// void Fit::set_parameters(const Info &info) {
//     pfnc_->SetParNames("a1", "a2", "b");
//     pfnc_->SetParLimits(0, -1.0e4, 1.0e4);
//     pfnc_->SetParLimits(1, -1.0e4, 1.0e4);
//     pfnc_->SetParLimits(2, 0.0, 1.0);
//     pfnc_->SetParameters(info.a1_in, info.a2_in, info.b_in);
//     pfnc_->FixParameter(1, 0.0);
//     std::cout << "set_parameters: done.\n";
// }
}  // namespace gg2aa
