/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

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

std::pair<int, InputFiles> check_files(const InputFiles &fnames) {
    int bad = 0;
    InputFiles failed;
    for (const auto &fname : fnames) {
        auto f = std::make_unique<std::ifstream>(fname);
        if (!f->good()) {
            ++bad;
            failed.push_back(fname);
        }
        f->close();
    }
    return make_pair(bad, failed);
}

std::pair<int, InputFiles> InputData::check_input() const {
    int bad = 0;
    InputFiles failed;

    // check signal files.
    auto f_sig = check_files(signal_);
    bad += f_sig.first;
    failed.insert(failed.end(), f_sig.second.begin(), f_sig.second.end());

    // check background files.
    for (const auto &bs : background_) {
        auto f_bg = check_files(bs.second);
        bad += f_bg.first;
        failed.insert(failed.end(), f_bg.second.begin(), f_bg.second.end());
    }

    // check template files.
    InputFiles tmp_files;
    for (const auto &t : templates_) { tmp_files.push_back(t.file_name()); }
    auto f_tmp = check_files(tmp_files);
    bad += f_tmp.first;
    failed.insert(failed.end(), f_tmp.second.begin(), f_tmp.second.end());

    return std::make_pair(bad, failed);
}
}  // namespace gg2aa
