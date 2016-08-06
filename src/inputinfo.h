#ifndef GGTOPAA_SRC_INPUTINFO_H_
#define GGTOPAA_SRC_INPUTINFO_H_

#include <ostream>

namespace gg2aa {
struct InputInfo {
    double rs, lum, eff, kg;
    double sig_direct, sig_one_frag, sig_two_frag;
    double bin_size, xlow, xup;
    double a1_in, a2_in, b_in;
    int status = 0;

    void show(std::ostream *out) const;
    void show_bg_summary(std::ostream *out) const;
    double sig_bg() const { return sig_direct + sig_one_frag + sig_two_frag; }
    int n_bg() const { return static_cast<int>(sig_bg() * lum * eff * 1.0e3); }
    int n_sig() const { return static_cast<int>(n_bg() * kg / (1.0 - kg)); }
    int nev() const { return n_bg() + n_sig(); }
};
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_INPUTINFO_H_
