#ifndef GGTOPAA_SRC_FIT_H_
#define GGTOPAA_SRC_FIT_H_

#include "info.h"
#include "templates.h"

namespace gg2aa {
class FitFunction {
public:
    FitFunction(const Template &t)
        : template_(t), sqrt_s_(t.sqrt_s()), nevent_(t.nevent()) {}
    ~FitFunction() {}

    double operator()(double *x, double *p);

private:
    const Template template_;
    const double sqrt_s_;
    const int nevent_;
};
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_FIT_H_
