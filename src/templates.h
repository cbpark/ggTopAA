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
    double norm() const;

    /** \\int (1-x^p)^a1 */
    friend double f_bg3(const Template &t, const double x, const double a1,
                        const double a2, const double p);
    /** \\int (1-x^(1/3))^a1 */
    friend double f_bg4(const Template &t, const double x, const double a1,
                        const double a2, const double p);

private:
    const std::string fname_;
    const std::pair<double, double> mass_width_;
    Range range_;
    double sqrt_s_ = 0.0;
    std::vector<double> maa_;
    double maa_interval_ = 0.0;
};

using Templates = std::vector<Template>;

double f_bg3(const gg2aa::Template &, const double, const double, const double,
             const double);

double f_bg4(const gg2aa::Template &, const double, const double, const double,
             const double);
}  // namespace gg2aa

#endif  // SRC_TEMPLATES_H_
