#include "parsers.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <istream>
#include <memory>
#include <string>
#include <vector>
#include "inputdata_type.h"
#include "types.h"

using std::ifstream;
using std::string;
using std::vector;
using std::unique_ptr;

namespace gg2aa {
vector<string> split(const string &str, char c) {
    vector<string> str_;
    auto i   = 0;
    auto pos = str.find(c);

    if (pos == string::npos) { str_.push_back(str); }

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
    } else if (str == "sigma") {
        data->set_status(InputStatus::SIGMA);
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
    } else if (s == InputStatus::SIGMA) {
        data->add_background("sigma", fname);
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

            auto pos         = line.find(":");
            auto parsed_size = parsed.size();
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

Sigma getSigma(const InputData &data) {
    Sigma sigma;
    const auto infiles = data.background("sigma");
    if (infiles.empty()) {
        sigma.status = -1;
        return sigma;
    }
    unique_ptr<ifstream> f(new ifstream(infiles.front()));
    if (!f->good()) {
        sigma.status = -2;
        return sigma;
    }

    *f >> sigma.rs >> sigma.lum >> sigma.eff >> sigma.kg;
    *f >> sigma.sig_direct >> sigma.sig_one_frag >> sigma.sig_two_frag;
    *f >> sigma.bin_size >> sigma.minbin >> sigma.maxbin;
    *f >> sigma.a1in >> sigma.a2in >> sigma.bin;
    return sigma;
}
}  // namespace gg2aa
