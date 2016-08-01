#ifndef GGTOPAA_SRC_HISTOGRAMS_H_
#define GGTOPAA_SRC_HISTOGRAMS_H_

#include <memory>
#include "TH1D.h"
#include "inputdata.h"

namespace gg2aa {
class Histograms {
public:
    explicit Histograms(const Sigma &sigma)
        : nbin_(sigma.nbin()), xlow_(sigma.minbin), xup_(sigma.maxbin) {
        bg_hist_  = TH1D("background", "", nbin_, xlow_, xup_);
        sig_hist_ = TH1D("signal", "", nbin_, xlow_, xup_);
    }
    explicit Histograms(int nbin, double xlow, double xup)
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

#endif  // GGTOPAA_SRC_HISTOGRAMS_H_
