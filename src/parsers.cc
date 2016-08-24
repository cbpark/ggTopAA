/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#include "parsers.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "info.h"
#include "inputdata.h"
#include "utils.h"

using std::string;
using std::vector;

namespace gg2aa {
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
    } else if (str == "template") {
        data->set_status(InputStatus::TEMPLATE);
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
    } else if (s == InputStatus::TEMPLATE) {
        data->add_template(fname);
    }
}

InputData parseInputData(std::unique_ptr<std::ifstream> is) {
    string line;
    vector<string> parsed;
    InputData data;
    while (getline(*is, line)) {
        if (line.find("#") == string::npos && !line.empty()) {
            parsed = removeSpaces(split(line, ':'));
            const auto pos = line.find(":");
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
    is->close();
    return data;
}

Info getInfo(const InputData &data) {
    Info info;
    const auto infiles = data.background("info");
    if (infiles.empty()) {
        info.status = -1;
        return info;
    }
    auto f = std::make_unique<std::ifstream>(infiles.front());
    if (!f->good()) {
        info.status = -2;
        return info;
    }

    *f >> info.rs >> info.lum >> info.eff >> info.kgg;
    *f >> info.sig_direct >> info.sig_one_frag >> info.sig_two_frag;
    *f >> info.bin_size >> info.xlow >> info.xup;
    *f >> info.s_in >> info.p_in >> info.b_in >> info.a0_in >> info.a1_in >>
        info.kgg_in;
    return info;
}
}  // namespace gg2aa
