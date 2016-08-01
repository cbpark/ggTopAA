#include "type.h"
#include <fstream>
#include <memory>
#include <string>
#include "TH1D.h"
#include "inputdata_type.h"

using std::ifstream;

namespace gg2aa {
template <typename T>
std::string get_string(std::string name, T x) {
    return name + " = " + std::to_string(x);
}

std::string Sigma::show() const {
    std::string str = "--- Sigma ---\n";
    str += "  " + get_string("Rs", rs * 1.0e-3) + " TeV, " +
           get_string("Lum", lum * 1.0e-3) + " ab^{-1}, " +
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

std::string Sigma::show_sig() const {
    std::string str = "--- Sig (pb) --- \n";
    str += "  " + get_string("Direct", sig_direct) + ", " +
           get_string("One fragment", sig_one_frag) + ", " +
           get_string("Two fragment", sig_two_frag) + "\n";
    return str;
}

std::string Sigma::show_bg_summary() const {
    const double sig_bg = sig_direct + sig_one_frag + sig_two_frag;

    const int n_bg  = static_cast<int>(sig_bg * lum * eff * 1.0e3);
    const int n_gg  = static_cast<int>(n_bg * kg / (1.0 - kg));
    const int n_evt = n_bg + n_gg;

    std::string str = "--- Info: backgrounds ---\n";
    str += "  " + get_string("Nevt", n_evt) + ", " + get_string("Nbg", n_bg) +
           ", " + get_string("Ngg", n_gg) + "\n";
    return str;
}

void Histograms::set_bg_hist(const InputData &data,
                             std::shared_ptr<Sigma> sigma) {
    double content;
    TH1D hist("hist", "", nbin_, xlow_, xup_);
    int n = 0, n_entries = 0;
    for (const auto &bg : data.background()) {
        if (bg.first == "sigma") { continue; }

        n = n_entries = 0;
        for (const auto &b : bg.second) {
            std::unique_ptr<ifstream> f(new ifstream(b));
            while (*f >> content) {
                hist.Fill(content);
                if (content > xlow_ && content < xup_) { ++n; }
                ++n_entries;
            }
        }
        if (bg.first == "direct") {
            sigma->sig_direct *= static_cast<double>(n) / n_entries;
            hist.Scale(sigma->sig_direct / n_entries);
            bg_hist_.Add(&hist);
        } else if (bg.first == "one-fragment") {
            sigma->sig_one_frag *= static_cast<double>(n) / n_entries;
            hist.Scale(sigma->sig_one_frag / n_entries);
            bg_hist_.Add(&hist);
        } else if (bg.first == "two-fragment") {
            sigma->sig_two_frag *= static_cast<double>(n) / n_entries;
            hist.Scale(sigma->sig_two_frag / n_entries);
            bg_hist_.Add(&hist);
        }
        hist.Reset();
    }
}
}  // namespace gg2aa
