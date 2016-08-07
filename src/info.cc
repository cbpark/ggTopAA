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
         << get_string("Eff", eff) << ", " << get_string("Kg", kg) << "\n";
    *out << "  " << get_string("sigDir", sig_direct) << ", "
         << get_string("sigOne", sig_one_frag) << ", "
         << get_string("sigTwo", sig_two_frag) << "\n";
    *out << "  " << get_string("bin size", bin_size) << ", "
         << "(xlow, xup) = "
         << "(" << xlow << ", " << xup << ")\n";
    *out << "  fit parameters: " << get_string("a1", a1_in) << ", "
         << get_string("a2", a2_in) << ", " << get_string("b", b_in) << "\n";
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