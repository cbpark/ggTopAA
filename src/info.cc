/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#include "info.h"
#include <ostream>
#include <string>

namespace gg2aa {
template <typename T>
std::string get_string(std::string name, T x) {
    return name + " = " + std::to_string(x);
}

void Info::show(std::ostream *out) const {
    *out << "--- Input information ---\n";
    *out << "  " << get_string("Rs", rs * 1.0e-3) << " TeV, "
         << get_string("Lum", lum * 1.0e-3) << " ab^{-1}, "
         << get_string("Eff", eff) << ", " << get_string("Kgg", kgg) << "\n";
    *out << "  " << get_string("sigDir", sig_direct) << ", "
         << get_string("sigOne", sig_one_frag) << ", "
         << get_string("sigTwo", sig_two_frag) << "\n";
    *out << "  " << get_string("bin size", bin_size) << ", "
         << "(xlow, xup) = "
         << "(" << xlow << ", " << xup << ")\n";
    *out << "  fit parameters: " << get_string("s", s_in) << ", "
         << get_string("p", p_in) << ", " << get_string("b", b_in) << ", "
         << get_string("a0", a0_in) << ", " << get_string("a1", a1_in) << ", "
         << get_string("kgg", kgg_in) << "\n";
}

void Info::show_bg_summary(std::ostream *out) const {
    *out << "--- Info: backgrounds ---\n";
    *out << "  Sigma (pb): " << get_string("Direct", sig_direct) << ", "
         << get_string("One fragment", sig_one_frag) << ", "
         << get_string("Two fragment", sig_two_frag) << '\n';
    *out << "  " << get_string("Nevt", nev()) << ", "
         << get_string("Nbg", n_bg()) << ", " << get_string("Ngg", n_sig())
         << "\n";
}
}  // namespace gg2aa
