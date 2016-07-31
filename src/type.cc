#include "type.h"
#include <string>

namespace gg2aa {
std::string get_string(std::string name, double x) {
    return name + " = " + std::to_string(x);
}

std::string Sigma::show() const {
    std::string str = "--- Sigma ---\n";
    str += "  " + get_string("Rs", rs) + ", " + get_string("Lum", lum) + ", " +
           get_string("Eff", eff) + ", " + get_string("Kg", kg) + "\n";
    str += "  " + get_string("sigDir", sig_direct) + ", " +
           get_string("sigOne", sig_one_frag) + ", " +
           get_string("sigTwo", sig_two_frag) + "\n";
    str += "  " + get_string("mbin", bin_size) + ", " +
           get_string("min", minbin) + ", " + get_string("max", maxbin) + ", " +
           get_string("nbin", nbin()) + "\n";
    str += "  " + get_string("a1in", a1in) + ", " + get_string("a2in", a2in) +
           ", " + get_string("bin", bin) + "\n";
    return str;
}
}  // namespace gg2aa
