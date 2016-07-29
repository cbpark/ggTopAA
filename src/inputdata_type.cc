#include "inputdata_type.h"
#include <string>

using std::string;

namespace gg2aa {
const string InputData::show_signal() const {
    string data = "signal (";
    if (signal_.empty()) {
        data += "):\n  No files.\n";
    } else {
        auto n = signal_.size();
        if (n == 1) {
            data += "1 file):\n";
        } else {
            data += std::to_string(n) + " files):\n";
        }
        for (const auto &d : signal_) { data += "  " + d + '\n'; }
    }
    return data;
}

const string InputData::show_background(string k) const {
    string data = "backgrounds (" + k;
    if (background_.find(k) == background_.end()) {
        data += "):\n  No files.\n";
    } else {
        auto n = background_.at(k).size();
        if (n == 1) {
            data += ", 1 file):\n";
        } else {
            data += ", " + std::to_string(n) + " files):\n";
        }
        for (const auto &d : background_.at(k)) { data += "  " + d + '\n'; }
    }
    return data;
}

const string InputData::show() const {
    return show_signal() + show_background("sigma") +
           show_background("direct") + show_background("fragment");
}

const string InputData::show_status() const {
    if (status_ == InputStatus::SIGNAL) {
        return "signal";
    } else if (status_ == InputStatus::SIGMA) {
        return "sigma";
    } else if (status_ == InputStatus::DIRECT) {
        return "direct";
    } else if (status_ == InputStatus::FRAGMENT) {
        return "fragment";
    } else {
        return "none";
    }
}
}  // namespace gg2aa
