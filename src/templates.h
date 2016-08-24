/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_TEMPLATES_H_
#define SRC_TEMPLATES_H_

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
    double norm_sig() const;

    friend double fit_func_bg1(const Template &, const double, const double,
                               const double, const double, const double);
    friend double fit_func_bg2(const Template &, const double, const double,
                               const double, const double, const double);
    friend double fit_func_bg3(const Template &, const double, const double,
                               const double, const double, const double);
    friend double fit_func_bg4(const Template &, const double, const double,
                               const double, const double, const double);
    friend double fit_func_bg5(const Template &, const double, const double,
                               const double, const double, const double);

private:
    const std::string fname_;
    const std::pair<double, double> mass_width_;
    Range range_;
    double sqrt_s_ = 0.0;
    std::vector<double> maa_;
    double maa_interval_ = 0.0;

    friend double fit_func_bg_nolog(const Template &, const double,
                                    const double, const double, const double);
};

using Templates = std::vector<Template>;

/** \\int (1-x^{1/3})^b * x^{a0} */
double fit_func_bg1(const Template &t, const double x, const double p,
                    const double b, const double a0, const double a1);

/** \\int (1-x^p)^b * x^{a0} */
double fit_func_bg2(const Template &, const double, const double, const double,
                    const double, const double);

/** \\int (1-x^p)^b */
double fit_func_bg3(const Template &, const double, const double, const double,
                    const double, const double);

/** \\int (1-x^{1/3})^b */
double fit_func_bg4(const Template &, const double, const double, const double,
                    const double, const double);

/** \\int (1-x^{1/3})^b) * x^{a0 + a1 * \\log(x)} */
double fit_func_bg5(const Template &, const double, const double, const double,
                    const double, const double);
}  // namespace gg2aa

#endif  // SRC_TEMPLATES_H_
