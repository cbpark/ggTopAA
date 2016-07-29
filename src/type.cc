#include "type.h"
#include <string>

using std::string;

namespace gg2aa {
const string InputData::show_signal() const {
    string data = "signal:\n";
    for (const auto &d : signal_) {
        data += "  " + d + '\n';
    }
    return data;
}

const string InputData::show_background(string k) const {
    string data = "backgrounds (" + k + "):\n";
    for (const auto &d : background_.at(k)) {
        data += "  " + d + '\n';
    }
    return data;
}
}  // namespace gg2aa
