#include "inputdata_type.h"
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
}  // namespace gg2aa
