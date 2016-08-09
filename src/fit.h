#ifndef SRC_FIT_H_
#define SRC_FIT_H_

#include <memory>
#include "TF1.h"
#include "TH1D.h"
#include "info.h"
#include "templates.h"
#include "utils.h"

namespace gg2aa {
class FitFunction {
public:
    FitFunction() = delete;
    explicit FitFunction(const Template &t, const Info &info)
        : template_(t),
          range_(Range(info.xlow, info.xup)),
          sqrt_s_(info.rs),
          nevent_(info.nev()),
          nbins_(info.num_bins()) {}
    ~FitFunction() {}

    Range range() const { return range_; }
    // The histogram fit will call this.
    double operator()(double *x, double *p) const;

private:
    const Template template_;
    const Range range_;
    const double sqrt_s_;
    const int nevent_, nbins_;
};

class Fit {
public:
    Fit(const FitFunction &f, const Info &info)
        : pfnc_(std::make_shared<TF1>("pfnc", f, f.range().low(),
                                      f.range().up(), 3)) {
        set_parameters(info);
    }
    ~Fit() {}

    /** Set parameters for fitting. */
    void set_parameters(const Info &info);

    double get_chisquare(std::shared_ptr<TH1D> hist) {
        hist->Fit(pfnc_.get(), "RN");
        return pfnc_->GetChisquare();
    }

private:
    std::shared_ptr<TF1> pfnc_;
};
}  // namespace gg2aa

#endif  // SRC_FIT_H_
