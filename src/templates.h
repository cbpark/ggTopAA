#ifndef GGTOPAA_SRC_TEMPLATES_H_
#define GGTOPAA_SRC_TEMPLATES_H_

#include <map>
#include <string>
#include <vector>
#include "histogram.h"
#include "input.h"

namespace gg2aa {
class Template {
public:
    Template(const std::string &fname, const InputInfo &info)
        : fname_(fname),
          range_(HistRange(info.xlow, info.xup)),
          sqrt_s_(info.rs) {
        set_maa();
    }
    ~Template() {}

    double f_maa(double x) const;
    double norm() const;

private:
    const std::string fname_;
    const HistRange range_;
    const double sqrt_s_;
    std::map<double, double> maa_;
    double maa_interval_;

    void set_maa();
};

using Templates = std::vector<Template>;
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_TEMPLATES_H_
