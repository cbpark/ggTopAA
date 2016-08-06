#include "inputdata.h"
#include <fstream>  // ifstream
#include <memory>   // unique_ptr
#include <ostream>
#include <string>
#include <utility>  // pair

using std::string;
using std::ostream;

namespace gg2aa {
void InputData::show_signal(ostream *out) const {
    *out << "signal (";
    if (signal_.empty()) {
        *out << "):\n  No files.\n";
    } else {
        const auto n = signal_.size();
        if (n == 1) {
            *out << "1 file):\n";
        } else {
            *out << n << " files):\n";
        }
        for (const auto &d : signal_) { *out << "  " << d << '\n'; }
    }
}

void InputData::show_background(string k, ostream *out) const {
    *out << "backgrounds (" << k;
    if (background_.find(k) == background_.end()) {
        *out << "):\n  No files.\n";
    } else {
        const auto n = background_.at(k).size();
        if (n == 1) {
            *out << ", 1 file):\n";
        } else {
            *out << ", " << n << " files):\n";
        }
        for (const auto &d : background_.at(k)) { *out << "  " << d << '\n'; }
    }
}

void InputData::show_templates(ostream *out) const {
    *out << "templates (";
    if (templates_.empty()) {
        *out << "):\n No files.\n";
    } else {
        const auto n = templates_.size();
        if (n == 1) {
            *out << "1 file):\n";
        } else {
            *out << n << " files):\n";
        }
        for (const auto &t : templates_) {
            *out << "  " << t.file_name() << '\n';
        }
    }
}

void InputData::show(ostream *out) const {
    *out << "--- Input data ---\n";
    show_signal(out);
    show_background("info", out);
    show_background("direct", out);
    show_background("one-fragment", out);
    show_background("two-fragment", out);
    show_templates(out);
}

void InputData::show_status(ostream *out) const {
    *out << "--- Current status: ";
    if (status_ == InputStatus::SIGNAL) {
        *out << "signal\n";
    } else if (status_ == InputStatus::INFO) {
        *out << "info\n";
    } else if (status_ == InputStatus::DIRECT) {
        *out << "direct\n";
    } else if (status_ == InputStatus::FRAGMENT1) {
        *out << "one-fragment\n";
    } else if (status_ == InputStatus::FRAGMENT2) {
        *out << "two-fragment\n";
    } else if (status_ == InputStatus::TEMPLATE) {
        *out << "template\n";
    } else {
        *out << "none\n";
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

    for (const auto &t: templates_) {
        unique_ptr<ifstream> f(new ifstream(t.file_name()));
        if (!f->good()) {
            ++bad;
            failed.push_back(t.file_name());
        }
    }

    return std::make_pair(bad, failed);
}

template <typename T>
string get_string(string name, T x) {
    return name + " = " + std::to_string(x);
}

void HistBin::show(std::ostream *out) const {
    *out << "  " << get_string("bin size", bin_size_) << ", "
         << "(xlow, xup) = "
         << "(" << xlow_ << ", " << xup_ << ")\n";
}

void InputInfo::show(ostream *out) const {
    *out << "--- Input info ---\n";
    *out << "  " << get_string("Rs", rs * 1.0e-3) << " TeV, "
         << get_string("Lum", lum * 1.0e-3) << " ab^{-1}, "
         << get_string("Eff", eff) << ", " << get_string("Kg", kg) << "\n";
    *out << "  " << get_string("sigDir", sig_direct) << ", "
         << get_string("sigOne", sig_one_frag) << ", "
         << get_string("sigTwo", sig_two_frag) << "\n";
    bins.show(out);
    *out << "  " << get_string("a1in", a1in) << ", " << get_string("a2in", a2in)
         << ", " << get_string("bin", bin) << "\n";
}

void InputInfo::show_bg_summary(ostream *out) const {
    *out << "--- Info: backgrounds ---\n";
    *out << "  Sigma (pb): " << get_string("Direct", sig_direct) << ", "
         << get_string("One fragment", sig_one_frag) << ", "
         << get_string("Two fragment", sig_two_frag) << '\n';
    *out << "  " << get_string("Nevt", nev()) << ", "
         << get_string("Nbg", n_bg()) << ", " << get_string("Ngg", n_sig())
         << "\n";
}
}  // namespace gg2aa
