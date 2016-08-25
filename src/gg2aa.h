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

int howToUse(const std::string &appname) {
    std::cerr << "Usage: " << appname << " input output fit_choice\n\n"
              << "    input      - input file\n"
              << "    output     - output file\n"
              << "    fit_choice - choice of fit function [1, ..., 6]\n\n"
              << "    ex) " << appname << " input.yml output.dat 6\n";
    return 1;
}

bool correctFitChoice(const int n) { return n > 0 && n < 7; }

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
