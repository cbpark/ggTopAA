#ifndef GGTOPAA_SRC_TYPES_H_
#define GGTOPAA_SRC_TYPES_H_

#include <memory>
#include <string>
#include "TH1D.h"
#include "inputdata_type.h"

namespace gg2aa {
struct Sigma {
    double rs, lum, eff, kg;
    double sig_direct, sig_one_frag, sig_two_frag;
    double bin_size, minbin, maxbin;
    double a1in, a2in, bin;
    int status = 0;

    std::string show() const;
    int nbin() const { return static_cast<int>((maxbin - minbin) / bin_size); }
    std::string show_sig() const;
    std::string show_bg_summary() const;
};

class Histograms {
public:
    explicit Histograms(const Sigma &sigma)
        : nbin_(sigma.nbin()), xlow_(sigma.minbin), xup_(sigma.maxbin) {
        bg_hist_  = TH1D("background", "", nbin_, xlow_, xup_);
        sig_hist_ = TH1D("signal", "", nbin_, xlow_, xup_);
    }
    explicit Histograms(int nbin, int xlow, int xup)
        : nbin_(nbin), xlow_(xlow), xup_(xup) {
        bg_hist_  = TH1D("background", "", nbin_, xlow_, xup_);
        sig_hist_ = TH1D("signal", "", nbin_, xlow_, xup_);
    }
    ~Histograms() {}

    void set_bg_hist(const InputData &data, std::shared_ptr<Sigma> sigma);

private:
    TH1D bg_hist_;
    TH1D sig_hist_;

    int nbin_;
    double xlow_, xup_;
};
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_TYPES_H_
