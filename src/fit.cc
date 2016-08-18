/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#include "fit.h"
#include <iomanip>
#include <memory>
#include <ostream>
#include <vector>
#include "Math/MinimizerOptions.h"
#include "TFitResult.h"
#include "TH1D.h"
#include "info.h"
#include "templates.h"

namespace gg2aa {
double FitFunction::operator()(double *x, double *p) const {
    const double fgg = template_.f_maa(x[0]) / norm_;
    const double sqrt_s = info_.rs;
    double f = info_.nev() / info_.num_bins() * template_.range().width();
    f *= (1.0 - p[3]) / sqrt_s *
             func_(template_, x[0] / sqrt_s, p[0], p[1], p[2]) +
         p[3] * fgg;
    return f;
}

void FitResult::write(std::shared_ptr<std::ostream> os) const {
    *os << std::fixed;
    *os << std::setw(9) << std::setprecision(2) << mass_;
    *os << std::setw(8) << std::setprecision(2) << width_;
    *os << std::setw(12) << std::setprecision(4) << chi2_;
    for (const auto p : par_) {
        *os << std::setw(11) << std::setprecision(4) << p;
    }
    *os << '\n';
}

void Fit::set_parameters(const Info &info) {
    pfnc_->SetParNames("a1", "a2", "p", "kgg");
    pfnc_->SetParameters(info.a1_in, info.a2_in, info.p_in, info.kgg_in);
    pfnc_->SetParLimits(0, 0, 1000);
    pfnc_->FixParameter(1, 0);
    pfnc_->SetParLimits(2, 1.0e-9, 10);
    pfnc_->SetParLimits(3, 0, 1);
}

void Fit::do_fit(std::shared_ptr<TH1D> hist,
                 std::shared_ptr<FitResult> result) {
    ROOT::Math::MinimizerOptions opt;
    opt.SetDefaultMinimizer("Minuit2", "Minimize");
    opt.SetMaxFunctionCalls(1000000);
    opt.SetMaxIterations(100000);
    opt.SetTolerance(0.001);

    // - "I": use integral of function in bin instead of value at bin center.
    // - "N": do not store the graphics function, do not draw.
    // - "S": the result of the fit is returned in the TFitResultPtr.
    auto r = hist->Fit(pfnc_.get(), "INS");

    int npar = r->NPar();
    std::vector<double> par;
    for (int i = 0; i != npar; ++i) { par.push_back(r->Parameter(i)); }
    result->set_result(r->Chi2(), par);
}
}  // namespace gg2aa
