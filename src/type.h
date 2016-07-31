#ifndef GGTOPAA_SRC_TYPE_H_
#define GGTOPAA_SRC_TYPE_H_

#include <string>
//
#include <TH1D.h>

namespace gg2aa {
struct Sigma {
    double rs, lum, eff, kg;
    double sig_direct, sig_one_frag, sig_two_frag;
    double bin_size, minbin, maxbin;
    double a1in, a2in, bin;
    int status = 0;

    std::string show() const;
    int nbin() const { return static_cast<int>((maxbin - minbin) / bin_size); }
};

class Histograms {
public:
    Histograms() {}
    ~Histograms() {}

private:
    TH1D bg_hist_;
    TH1D sig_hist_;
};
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_TYPE_H_
