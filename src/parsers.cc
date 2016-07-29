#include "parsers.h"
#include <algorithm>
#include <cctype>
#include <istream>
#include <string>
#include <vector>
#include "inputdata_type.h"

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

string removeSpace(string str) {
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

vector<string> removeSpaces(const vector<string> &ss) {
    vector<string> ss_;
    for (const auto &s : ss) {
        if (!s.empty()) {
            ss_.push_back(removeSpace(s));
        }
    }
    return ss_;
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
    }
}

void addData(FileName fname, InputData *data) {
    if (fname.front() == '-') {
        fname = fname.substr(1, fname.length());
    }

    const InputStatus s = data->get_status();
    if (s == InputStatus::SIGNAL) {
        data->add_signal(fname);
    } else if (s == InputStatus::SIGMA) {
        data->add_background("sigma", fname);
    } else if (s == InputStatus::DIRECT) {
        data->add_background("direct", fname);
    } else if (s == InputStatus::FRAGMENT) {
        data->add_background("fragment", fname);
    }
}

InputData parseInputData(std::istream *is) {
    string line;
    vector<string> parsed;
    InputData data;

    while (getline(*is, line)) {
        if (line.find("#") == string::npos && !(line.empty())) {
            parsed           = removeSpaces(split(line, ':'));
            auto parsed_size = parsed.size();

            auto pos = line.find(":");
            if (pos != string::npos) {  // field found.
                setStatus(removeSpace(line.substr(0, pos)), &data);
                if (parsed_size == 1) {
                    continue;
                }
            }

            if (parsed_size == 2) {
                addData(parsed.back(), &data);
            } else if (parsed_size == 1) {
                addData(parsed.front(), &data);
            }
        } else {  // comment or empty line
            continue;
        }
    }

    return data;
}
}  // namespace gg2aa
