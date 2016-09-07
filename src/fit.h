/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_FIT_H_
#define SRC_FIT_H_

#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include "TF1.h"
#include "TGraph2D.h"
#include "TH1D.h"
#include "info.h"
#include "templates.h"
#include "utils.h"

namespace gg2aa {
class FitFunction {
private:
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

using FitParameters = std::array<double, 6>;

class FitResult {
public:
    FitResult() {}
    explicit FitResult(const Template &t)
        : mass_(t.mass_width().first), width_(t.mass_width().second) {}
    ~FitResult() {}

    double mass() const { return mass_; }
    double width() const { return width_; }
    double chi2() const { return chi2_; }
    FitParameters parameter() const { return par_; }
    void set_result(const FitParameters &par, const double chi2,
                    const unsigned int ndf, const int status) {
        par_ = par;
        chi2_ = chi2;
        chi2_ndf_ = chi2_ / ndf;
        status_ = status;
    }

    friend std::istream &operator>>(std::istream &is, FitResult &fres);
    friend std::ostream &operator<<(std::ostream &os, const FitResult &fres);

private:
    double mass_ = 0;
    double width_ = 0;
    double chi2_ = 0, chi2_ndf_ = 0;
    FitParameters par_;
    int status_ = 0;
};

class Fit {
public:
    Fit() = delete;
    explicit Fit(const FitFunction &f)
        : pfnc_(std::make_shared<TF1>("pfnc", f, f.range().low(),
                                      f.range().up(), FitParameters().size())) {
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

/**
 *  The result of fitting is stored in the function using
 *  TGraph2D::Interpolate of ROOT.
 */
class FitResultFunc {
public:
    FitResultFunc() = delete;
    explicit FitResultFunc(const std::vector<FitResult> &fres)
        : fres_graph_(mkGraph2D(fres)) {}
    ~FitResultFunc() {}

    double operator()(const double *x) const {
        return fres_graph_->Interpolate(x[0], x[1]);
    }

private:
    std::shared_ptr<TGraph2D> fres_graph_;
    std::shared_ptr<TGraph2D> mkGraph2D(const std::vector<FitResult> &fres);
};

std::pair<std::array<double, 2>, double> minPoint(
    std::function<double(const double *)> frfunc);

}  // namespace gg2aa

#endif  // SRC_FIT_H_
