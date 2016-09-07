/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#include "gg2aa.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "fit.h"
#include "parsers.h"

int main(int, char *argv[]) {
    const std::string appname("gg2aa_contour");

    auto infile = std::make_unique<std::ifstream>(argv[1]);
    if (!infile->good()) { return failedToRead(appname, argv[1]); }

    auto fres = gg2aa::parseFitResults(std::move(infile));
    auto frfunc = gg2aa::FitResultFunc(fres);
    auto point = gg2aa::minPoint(frfunc);
    std::cout << "Minimum: " << point.first[0] << ", " << point.first[1] << ", "
              << point.second << '\n';
}
