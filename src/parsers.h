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
