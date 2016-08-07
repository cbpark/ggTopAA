#include "utils.h"
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using std::string;

namespace gg2aa {
std::vector<string> split(const string &str, char c) {
    std::vector<string> str_;
    auto pos = str.find(c);
    if (pos == string::npos) { str_.push_back(str); }

    auto i = 0;
    while (pos != string::npos) {
        str_.push_back(str.substr(i, pos - i));
        i = ++pos;
        pos = str.find(c, pos);

        if (pos == string::npos) {
            str_.push_back(str.substr(i, str.length()));
        }
    }
    return str_;
}

string fileName(const string &path) {
    return path.substr(path.find_last_of('/') + 1);
}

string baseName(const string &path, const string &ext) {
    string fname(fileName(path));
    string tmp_ext = ext;
    tmp_ext.insert(0, 1, '.');
    return fname.substr(0, fname.find_last_of(tmp_ext) - ext.size());
}

double stringToDouble(const string &str) {
    double x;
    std::istringstream iss(str);
    iss >> x;
    if (iss.bad() || iss.fail()) { return 0.0; }
    return x;
}

std::pair<double, double> getMassWidth(const string &fname) {
    string bname(baseName(fname, "dat"));
    const auto splitted = split(bname, '_');
    if (splitted.size() < 3) { return std::make_pair(0.0, 0.0); }
    return std::make_pair(stringToDouble(splitted[1]),
                          stringToDouble(splitted[2]));
}
}  // namespace gg2aa
