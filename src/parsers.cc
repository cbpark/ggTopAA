#include "parsers.h"
#include <algorithm>
#include <cctype>
#include <istream>
#include <string>
#include <vector>
#include "type.h"

using std::string;
using std::vector;

namespace gg2aa {
vector<string> split(const string &str, char c) {
    vector<string> str_;
    auto i   = 0;
    auto pos = str.find(c);
    if (pos == string::npos) {
        str_.push_back(str);
    }
    while (pos != string::npos) {
        str_.push_back(str.substr(i, pos - i));
        i   = ++pos;
        pos = str.find(c, pos);

        if (pos == string::npos) {
            str_.push_back(str.substr(i, str.length()));
        }
    }
    return str_;
}

vector<string> removeSpaces(const vector<string> &ss) {
    auto ss_ = ss;
    for (auto &s : ss_) {
        s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
    }
    return ss_;
}

string removeInitDash(const string &str) {
    if (str.front() == '-') {
        return str.substr(1, str.length());
    }
    return str;
}

void setStatus(string str, InputData *data) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    if (str == "signal") {
        data->set_status(InputStatus::SIGNAL);
    } else if (str == "sigma") {
        data->set_status(InputStatus::SIGMA);
    } else if (str == "direct") {
        data->set_status(InputStatus::DIRECT);
    } else if (str == "fragment") {
        data->set_status(InputStatus::FRAGMENT);
    } else {
        data->set_status(InputStatus::NONE);
    }
}

InputData parseInputData(std::istream *is) {
    string line;
    vector<string> parsed;
    InputData data;

    while (getline(*is, line)) {
        if (line.find("#") == string::npos && !(line.empty())) {
            parsed = removeSpaces(split(line, ':'));
            setStatus(parsed.front(), &data);
        } else {  // comment or empty line
            continue;
        }
    }

    return data;
}
}  // namespace gg2aa
