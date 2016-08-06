#ifndef GGTOPAA_SRC_FIT_FUNC_H_
#define GGTOPAA_SRC_FIT_FUNC_H_

#include "inputdata.h"
#include "templates.h"

namespace gg2aa {
class FitFunction {
public:
    explicit FitFunction(const InputInfo &info, const Template &temp)
        : template_(temp),
          nevent_(info.nev()),
          num_bins_(info.bins.num_bins()) {}
    ~FitFunction() {}

    double operator()(double *x, double *p);

private:
    Template template_;
    int nevent_;
    int num_bins_;
};
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_FIT_FUNC_H_
