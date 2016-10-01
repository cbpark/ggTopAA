/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#include "gg2aa.h"
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include "parsers.h"

using std::string;

int main(int argc, char *argv[]) {
    const string appname("gg2aa_bestfits");
    if (argc != 3) {
        string usage = "Usage: " + appname + " input output\n\n";
        usage += "    input  - input data file\n";
        usage += "    output - output file in the root format\n\n";
        usage += "    ex) " + appname + " bestfit.dat output.root\n";
        return howToUse(usage);
    }

    auto infile = std::make_unique<std::ifstream>(argv[1]);
    if (!infile->good()) { return failedToRead(appname, argv[1]); }
    const auto to_out = &std::cout;  // information will be displayed in screen.

    auto bestfits = gg2aa::parseBestFitPoints(std::move(infile));
    for (const auto &p : bestfits) { *to_out << p << '\n'; }
}
