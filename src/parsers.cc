#include "parsers.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <istream>
#include <memory>
#include <string>
#include <vector>
#include "histograms.h"
#include "inputdata.h"

using std::ifstream;
using std::string;
using std::vector;
using std::unique_ptr;

namespace gg2aa {
vector<string> split(const string &str, char c) {
    vector<string> str_;
    auto pos = str.find(c);
    if (pos == string::npos) { str_.push_back(str); }

    auto i = 0;
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
        if (!s.empty()) { ss_.push_back(removeSpace(s)); }
    }
    return ss_;
}

void setInputStatus(string str, InputData *data) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    if (str == "signal") {
        data->set_status(InputStatus::SIGNAL);
    } else if (str == "info") {
        data->set_status(InputStatus::INFO);
    } else if (str == "direct") {
        data->set_status(InputStatus::DIRECT);
    } else if (str == "one-fragment") {
        data->set_status(InputStatus::FRAGMENT1);
    } else if (str == "two-fragment") {
        data->set_status(InputStatus::FRAGMENT2);
    }
}

void addInputData(FileName fname, InputData *data) {
    if (fname.front() == '-') { fname = fname.substr(1, fname.length()); }

    const auto s = data->get_status();
    if (s == InputStatus::SIGNAL) {
        data->add_signal(fname);
    } else if (s == InputStatus::INFO) {
        data->add_background("info", fname);
    } else if (s == InputStatus::DIRECT) {
        data->add_background("direct", fname);
    } else if (s == InputStatus::FRAGMENT1) {
        data->add_background("one-fragment", fname);
    } else if (s == InputStatus::FRAGMENT2) {
        data->add_background("two-fragment", fname);
    }
}

InputData parseInputData(unique_ptr<std::istream> is) {
    string line;
    vector<string> parsed;
    InputData data;
    while (getline(*is, line)) {
        if (line.find("#") == string::npos && !line.empty()) {
            parsed = removeSpaces(split(line, ':'));

            const auto pos         = line.find(":");
            const auto parsed_size = parsed.size();
            if (pos != string::npos) {  // field found.
                setInputStatus(removeSpace(line.substr(0, pos)), &data);
                if (parsed_size == 1) { continue; }
            }

            if (parsed_size == 2) {
                addInputData(parsed.back(), &data);
            } else if (parsed_size == 1) {
                addInputData(parsed.front(), &data);
            }
        } else {  // comment or empty line
            continue;
        }
    }
    return data;
}

InputInfo getInputInfo(const InputData &data) {
    InputInfo info;
    const auto infiles = data.background("info");
    if (infiles.empty()) {
        info.status = -1;
        return info;
    }
    unique_ptr<ifstream> f(new ifstream(infiles.front()));
    if (!f->good()) {
        info.status = -2;
        return info;
    }

    *f >> info.rs >> info.lum >> info.eff >> info.kg;
    *f >> info.sig_direct >> info.sig_one_frag >> info.sig_two_frag;
    *f >> info.bins.size >> info.bins.xlow >> info.bins.xup;
    *f >> info.a1in >> info.a2in >> info.bin;
    return info;
}
}  // namespace gg2aa
