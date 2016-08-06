#ifndef GGTOPAA_SRC_FIT_FUNC_H_
#define GGTOPAA_SRC_FIT_FUNC_H_

#include "histograms.h"

namespace gg2aa {
class FitFunction {
public:
    explicit FitFunction(const Histograms &hist) : hist_(hist) {}
    ~FitFunction() {}

    double operator()(double *x, double *p);

private:
    Histograms hist_;
};
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_FIT_FUNC_H_
