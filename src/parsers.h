#ifndef GGTOPAA_SRC_PARSERS_H_
#define GGTOPAA_SRC_PARSERS_H_

#include <istream>
#include <memory>
#include "inputdata_type.h"
#include "type.h"

namespace gg2aa {
InputData parseInputData(std::unique_ptr<std::istream> is);

Sigma getSigma(const InputData &data);
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_PARSERS_H_
