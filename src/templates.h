#ifndef SRC_TEMPLATES_H_
#define SRC_TEMPLATES_H_

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "info.h"
#include "utils.h"

namespace gg2aa {
class Template {
public:
    Template() = delete;
    explicit Template(const std::string &fname)
        : fname_(fname), mass_width_(getMassWidth(fname)) {}
    ~Template() {}

    void set_template(const Info &info);
    std::string file_name() const { return fname_; }
    std::pair<double, double> mass_width() const { return mass_width_; }
    /** Range of m_aa in the template data. */
    Range range() const { return range_; }
    double f_maa(double x) const;  // corresponds to sfnc(...) function
                                   // in the original implementation.
    /** Normalization factor for the given template data. */
    double norm() const;

    friend double fATL(const Template &t, double x, double a1, double a2);

private:
    const std::string fname_;
    const std::pair<double, double> mass_width_;
    Range range_;
    double sqrt_s_ = 0.0;
    std::map<double, double> maa_;
    double maa_interval_ = 0.0;
};

using Templates = std::vector<Template>;
}  // namespace gg2aa

#endif  // SRC_TEMPLATES_H_
