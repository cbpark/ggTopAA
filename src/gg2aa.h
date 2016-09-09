/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_GG2AA_H_
#define SRC_GG2AA_H_

#include <iostream>
#include <string>

int howToUse(const std::string &usage) {
    std::cerr << usage << '\n';
    return 1;
}

bool correctChoice(const int n, const int upper) { return n > 0 && n <= upper; }

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

void write_header(std::ostream *os) {
    *os << "# mass, width, chi2, chi2/ndf, parameters (s, p, b, a0, a1, kgg)";
    *os << ", status (>2 means invalid result)\n";
}

#endif  // SRC_GG2AA_H_
