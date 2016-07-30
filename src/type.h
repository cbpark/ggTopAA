#ifndef GGTOPAA_SRC_TYPE_H_
#define GGTOPAA_SRC_TYPE_H_

#include <string>

namespace gg2aa {
struct Sigma {
    double rs, lum, eff, kg;
    double sig_direct, sig_one_frag, sig_two_frag;
    double bin_size, minbin, maxbin;
    double a1in, a2in, bin;

    int status = 0;

    const std::string show() const;
};
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_TYPE_H_
