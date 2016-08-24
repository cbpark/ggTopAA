/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_FIT_H_
#define SRC_FIT_H_

#include <functional>
#include <memory>
#include <ostream>
#include <vector>
#include "TF1.h"
#include "TH1D.h"
#include "info.h"
#include "templates.h"
#include "utils.h"

namespace gg2aa {
class FitFunction {
    // this is just a type synonym.
    using FFType = std::function<double(
        const Template &, const double, const double, const double,
        const double, const double, const double)>;

public:
    FitFunction() = delete;
    FitFunction(const Template &t, const Info &info, const FFType &func)
        : template_(t), norm_sig_(t.norm_sig()), info_(info), func_(func) {}
    ~FitFunction() {}

    Info info() const { return info_; }
    Range range() const { return Range(info_.xlow, info_.xup); }
    // The histogram fit will call this.
    double operator()(double *x, double *p) const;

private:
    const Template template_;
    const double norm_sig_;
    const Info info_;
    FFType func_;
};

class FitResult {
public:
    FitResult() = delete;
    explicit FitResult(const Template &t)
        : mass_(t.mass_width().first), width_(t.mass_width().second) {}
    ~FitResult() {}

    double chi2() const { return chi2_; }
    std::vector<double> parameter() const { return par_; }
    void set_result(const std::vector<double> &par, const double chi2,
                    const unsigned int ndf) {
        par_ = par;
        chi2_ = chi2;
        chi2_ndf_ = chi2_ / ndf;
    }
    void write(std::shared_ptr<std::ostream> os) const;

private:
    const double mass_;
    const double width_;
    double chi2_ = 0, chi2_ndf_ = 0;
    std::vector<double> par_;
};

class Fit {
public:
    Fit() = delete;
    explicit Fit(const FitFunction &f)
        : pfnc_(std::make_shared<TF1>("pfnc", f, f.range().low(),
                                      f.range().up(), 6)) {
        init_parameters(f.info());
    }
    ~Fit() {}

    /** Fit function object. */
    std::shared_ptr<TF1> pfnc() { return pfnc_; }

    /**
     * Perform fitting histogram and return chi square and
     * the best fit parameters.
     */
    void do_fit(std::shared_ptr<TH1D> hist, std::shared_ptr<FitResult> result);

private:
    std::shared_ptr<TF1> pfnc_;
    void init_parameters(const Info &info);
};

Fit mkFit(const Template &t, const Info &info, const int fit_choice);

Fit fit1(const Template &t, const Info &info);
Fit fit2(const Template &t, const Info &info);
Fit fit3(const Template &t, const Info &info);
Fit fit4(const Template &t, const Info &info);
Fit fit5(const Template &t, const Info &info);
Fit fit6(const Template &t, const Info &info);
}  // namespace gg2aa

#endif  // SRC_FIT_H_
