/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_INPUTDATA_H_
#define SRC_INPUTDATA_H_

#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "info.h"
#include "templates.h"

namespace gg2aa {
using FileName = std::string;
using InputFiles = std::vector<FileName>;
using Backgrounds = std::unordered_map<std::string, InputFiles>;

enum class InputStatus {
    NONE,
    INFO,
    DIRECT,
    FRAGMENT1,
    FRAGMENT2,
    SIGNAL,
    TEMPLATE
};

class InputData {
public:
    InputData() {}
    ~InputData() {}

    InputFiles signal() const { return signal_; }
    void add_signal(const FileName &fname) { signal_.push_back(fname); }

    Backgrounds background() const { return background_; }
    InputFiles background(std::string k) const { return background_.at(k); }
    void add_background(std::string k, const FileName &fname) {
        background_[k].push_back(fname);
    }

    Templates templates() const { return templates_; }
    void add_template(const FileName &fname) {
        templates_.push_back(Template(fname));
    }
    void set_templates(const Info &info) {
        for (auto &t : templates_) { t.set_template(info); }
    }

    void show(std::ostream *out) const;

    InputStatus get_status() const { return status_; }
    void set_status(InputStatus s) { status_ = s; }
    void reset_status() { status_ = InputStatus::NONE; }
    void show_status(std::ostream *out) const;

    std::pair<int, InputFiles> check_input() const;

private:
    InputFiles signal_;
    Backgrounds background_;
    Templates templates_;
    InputStatus status_ = InputStatus::NONE;

    void show_signal(std::ostream *out) const;
    void show_background(std::string k, std::ostream *out) const;
    void show_templates(std::ostream *out) const;
};
}  // namespace gg2aa

#endif  // SRC_INPUTDATA_H_
