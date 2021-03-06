/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_INFO_H_
#define SRC_INFO_H_

#include <cmath>
#include <ostream>

namespace gg2aa {
struct Info {
    double rs, lum, eff, kgg;
    double sig_direct, sig_one_frag, sig_two_frag;
    double bin_size, xlow, xup;
    double s_in, p_in, b_in, a0_in, a1_in, kgg_in;
    int status = 0;

    void show(std::ostream *out) const;
    void show_bg_summary(std::ostream *out) const;
    double sig_bg() const { return sig_direct + sig_one_frag + sig_two_frag; }
    int n_bg() const { return static_cast<int>(sig_bg() * lum * eff * 1.0e3); }
    int n_sig() const { return static_cast<int>(n_bg() * kgg / (1.0 - kgg)); }
    int nev() const { return n_bg() + n_sig(); }
    int num_bins() const {
        return static_cast<int>(std::fabs(xup - xlow) / bin_size);
    }
};
}  // namespace gg2aa

#endif  // SRC_INFO_H_
