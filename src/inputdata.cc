#include "inputdata.h"
#include <fstream>  // ifstream
#include <memory>   // unique_ptr
#include <string>
#include <utility>  // pair

using std::string;

namespace gg2aa {
string InputData::show_signal() const {
    string data = "signal (";
    if (signal_.empty()) {
        data += "):\n  No files.\n";
    } else {
        const auto n = signal_.size();
        if (n == 1) {
            data += "1 file):\n";
        } else {
            data += std::to_string(n) + " files):\n";
        }
        for (const auto &d : signal_) { data += "  " + d + '\n'; }
    }
    return data;
}

string InputData::show_background(string k) const {
    string data = "backgrounds (" + k;
    if (background_.find(k) == background_.end()) {
        data += "):\n  No files.\n";
    } else {
        const auto n = background_.at(k).size();
        if (n == 1) {
            data += ", 1 file):\n";
        } else {
            data += ", " + std::to_string(n) + " files):\n";
        }
        for (const auto &d : background_.at(k)) { data += "  " + d + '\n'; }
    }
    return data;
}

string InputData::show() const {
    return "--- Input data ---\n" + show_signal() + show_background("sigma") +
           show_background("direct") + show_background("one-fragment") +
           show_background("two-fragment");
}

string InputData::show_status() const {
    if (status_ == InputStatus::SIGNAL) {
        return "signal";
    } else if (status_ == InputStatus::SIGMA) {
        return "sigma";
    } else if (status_ == InputStatus::DIRECT) {
        return "direct";
    } else if (status_ == InputStatus::FRAGMENT1) {
        return "one-fragment";
    } else if (status_ == InputStatus::FRAGMENT2) {
        return "two-fragment";
    } else {
        return "none";
    }
}

std::pair<int, InputFiles> InputData::check_input() const {
    using std::unique_ptr;
    using std::ifstream;

    int bad = 0;
    InputFiles failed;

    for (const auto &s : signal_) {
        unique_ptr<ifstream> f(new ifstream(s));
        if (!f->good()) {
            ++bad;
            failed.push_back(s);
        }
    }

    for (const auto &bs : background_) {
        for (const auto &b : bs.second) {
            unique_ptr<ifstream> f(new ifstream(b));
            if (!f->good()) {
                ++bad;
                failed.push_back(b);
            }
        }
    }

    return std::make_pair(bad, failed);
}

template <typename T>
string get_string(string name, T x) {
    return name + " = " + std::to_string(x);
}

string Sigma::show() const {
    string str = "--- Input info ---\n";
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

string Sigma::show_sig() const {
    string str = "--- Sig (pb) --- \n";
    str += "  " + get_string("Direct", sig_direct) + ", " +
           get_string("One fragment", sig_one_frag) + ", " +
           get_string("Two fragment", sig_two_frag) + "\n";
    return str;
}

string Sigma::show_bg_summary() const {
    const double sig_bg = sig_direct + sig_one_frag + sig_two_frag;

    const int n_bg  = static_cast<int>(sig_bg * lum * eff * 1.0e3);
    const int n_gg  = static_cast<int>(n_bg * kg / (1.0 - kg));
    const int n_evt = n_bg + n_gg;

    string str = "--- Info: backgrounds ---\n";
    str += "  " + get_string("Nevt", n_evt) + ", " + get_string("Nbg", n_bg) +
           ", " + get_string("Ngg", n_gg) + "\n";
    return str;
}
}  // namespace gg2aa
