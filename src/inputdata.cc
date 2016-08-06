#include "inputdata.h"
#include <fstream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>

namespace gg2aa {
void InputData::show_signal(std::ostream *out) const {
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

void InputData::show_background(std::string k, std::ostream *out) const {
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

void InputData::show_templates(std::ostream *out) const {
    *out << "template (";
    if (templates_.empty()) {
        *out << "):\n  No files.\n";
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

void InputData::show(std::ostream *out) const {
    *out << "--- Input data ---\n";
    show_signal(out);
    show_background("info", out);
    show_background("direct", out);
    show_background("one-fragment", out);
    show_background("two-fragment", out);
    show_templates(out);
}

void InputData::show_status(std::ostream *out) const {
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

    return std::make_pair(bad, failed);
}
}  // namespace gg2aa
