#include "histogram.h"
#include <ostream>

namespace gg2aa {
void Histogram::show_info(std::ostream *out) const {
    *out << "  bin size = " << bin_size_ << ", "
         << "(xlow, xup) = "
         << "(" << xlow_ << ", " << xup_ << ")\n";
}
}  // namespace gg2aa
