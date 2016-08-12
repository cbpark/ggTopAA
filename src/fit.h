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

    double get_chisquare(std::shared_ptr<TH1D> hist) {
        hist->Fit(pfnc_.get(), "IN");  // "I": use integral of function in bin
                                       // instead of value at bin center.
                                       // "N": do not store the graphics
                                       // function, do not draw.
        return pfnc_->GetChisquare();
    }

private:
    std::shared_ptr<TF1> pfnc_;
};
}  // namespace gg2aa

#endif  // SRC_FIT_H_
