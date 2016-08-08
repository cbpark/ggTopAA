#ifndef GGTOPAA_SRC_FIT_H_
#define GGTOPAA_SRC_FIT_H_

#include <iostream>
#include <memory>
#include "TF1.h"
#include "TH1D.h"
#include "info.h"
#include "templates.h"
#include "utils.h"

namespace gg2aa {
class FitFunction {
public:
    explicit FitFunction(const Template &t, const Info &info)
        : template_(t),
          range_(Range(info.xlow, info.xup)),
          sqrt_s_(info.rs),
          nevent_(info.nev()),
          nbins_(info.num_bins()) {}
    ~FitFunction() {}

    // Range range() const { return range_; }
    double operator()(double *x, double *p);

private:
    const Template template_;
    const Range range_;
    const double sqrt_s_;
    const int nevent_, nbins_;
};

// class Fit {
// public:
//     Fit(const FitFunction &f, const Info &info)
//         : pfnc_(new TF1("pfnc", f, f.range().low(), f.range().up(), 3)) {
//         set_parameters(info);
//     }
//     ~Fit() { delete pfnc_; }

//     void set_parameters(const Info &info);
//     double do_fit(std::shared_ptr<TH1D> hist) {
//         std::cout << "do_fit: Here!\n";
//         hist->Fit(pfnc_, "I");
//         std::cout << "do_fit: Here! (2)\n";
//         return pfnc_->GetChisquare();
//     }

// private:
//     TF1 *pfnc_;
// };
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_FIT_H_
