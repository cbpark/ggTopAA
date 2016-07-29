#ifndef GGTOPAA_SRC_PARSERS_H_
#define GGTOPAA_SRC_PARSERS_H_

#include <istream>
#include "inputdata_type.h"

namespace gg2aa {
InputData parseInputData(std::istream *is);
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_PARSERS_H_
