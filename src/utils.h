/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <string>
#include <utility>
#include <vector>

namespace gg2aa {
class Range {
public:
    Range() {}
    Range(double x1, double x2) {
        if (x1 > x2) {
            low_ = x2;
            up_ = x1;
        } else {
            low_ = x1;
            up_ = x2;
        }
    }
    ~Range() {}

    double low() const { return low_; }
    double up() const { return up_; }
    double width() const { return up_ - low_; }
    bool includes(double x) const { return x >= low_ && x <= up_; }

private:
    double low_, up_;
};

std::vector<std::string> split(const std::string &str, char c);

/** Deduce the mass and the width of the input data by its file name. */
std::pair<double, double> getMassWidth(const std::string &fname);

/** Suppress warnings about unused variables. */
template <typename T>
void ignore(const T &) {}
}  // namespace gg2aa

#endif  // SRC_UTILS_H_
