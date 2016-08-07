#ifndef GGTOPAA_SRC_FIT_H_
#define GGTOPAA_SRC_FIT_H_

#include "info.h"
#include "templates.h"

namespace gg2aa {
class FitFunction {
public:
    FitFunction(const Template &t, const Info &info)
        : template_(t),
          sqrt_s_(info.rs),
          nevent_(info.nev()),
          nbins_(info.num_bins()) {}
    ~FitFunction() {}

    double operator()(double *x, double *p);

private:
    const Template template_;
    const double sqrt_s_;
    const int nevent_, nbins_;
};
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_FIT_H_
