/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_PARSERS_H_
#define SRC_PARSERS_H_

#include <istream>
#include <memory>
#include "info.h"
#include "inputdata.h"

namespace gg2aa {
InputData parseInputData(std::unique_ptr<std::istream> is);

Info getInfo(const InputData &data);
}  // namespace gg2aa

#endif  // SRC_PARSERS_H_
