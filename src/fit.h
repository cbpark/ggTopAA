/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_FIT_H_
#define SRC_FIT_H_

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
public:
    FitFunction() = delete;
    FitFunction(const Template &t, const Info &info)
        : template_(t), norm_(t.norm()), info_(info) {}
    ~FitFunction() {}

    Info info() const { return info_; }
    Range range() const { return Range(info_.xlow, info_.xup); }
    // The histogram fit will call this.
    double operator()(double *x, double *p) const;

private:
    const Template template_;
    const double norm_;
    const Info info_;
};

class FitResult {
public:
    FitResult() = delete;
    explicit FitResult(const Template &t)
        : mass_(t.mass_width().first), width_(t.mass_width().second) {}
    ~FitResult() {}

    double chi2() const { return chi2_; }
    std::vector<double> parameter() const { return par_; }
    void set_result(const double chi2, const std::vector<double> &par) {
        chi2_ = chi2;
        par_ = par;
    }
    void write(std::shared_ptr<std::ostream> os) const;

private:
    const double mass_;
    const double width_;
    double chi2_;
    std::vector<double> par_;
};

class Fit {
public:
    Fit() = delete;
    explicit Fit(const FitFunction &f)
        : pfnc_(std::make_shared<TF1>("pfnc", f, f.range().low(),
                                      f.range().up(), 3)) {
        set_parameters(f.info());
    }
    ~Fit() {}

    /** Set parameters for fitting. */
    void set_parameters(const Info &info);

    /**
     * Perform fitting histogram and return chi square and
     * the best fit parameters.
     */
    void do_fit(std::shared_ptr<TH1D> hist, std::shared_ptr<FitResult> result);

private:
    std::shared_ptr<TF1> pfnc_;
};
}  // namespace gg2aa

#endif  // SRC_FIT_H_
