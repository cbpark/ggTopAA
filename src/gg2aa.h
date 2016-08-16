/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_GG2AA_H_
#define SRC_GG2AA_H_

#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include "templates.h"

int failedToRead(const std::string &appname, const std::string &fname) {
    std::cerr << appname << ": failed to read `" << fname << "'.\n";
    return 1;
}

int errMsg(const std::string &appname, const std::string &msg) {
    std::cerr << appname << ": " << msg << '\n';
    return 1;
}

void message(const std::string &appname, const std::string &msg,
             std::ostream *out) {
    *out << appname << ": " << msg << '\n';
}

void writeChiSquare(const gg2aa::Template &t, double chi2,
                    std::shared_ptr<std::ostream> os) {
    *os << std::fixed;
    *os << std::setw(9) << std::setprecision(2) << t.mass_width().first;
    *os << std::setw(8) << std::setprecision(2) << t.mass_width().second;
    *os << std::setw(12) << std::setprecision(4) << chi2 << '\n';
}

#endif  // SRC_GG2AA_H_
