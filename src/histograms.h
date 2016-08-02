#ifndef GGTOPAA_SRC_HISTOGRAMS_H_
#define GGTOPAA_SRC_HISTOGRAMS_H_

#include <memory>
#include <vector>
#include "TH1D.h"
#include "inputdata.h"

namespace gg2aa {
class Histograms {
public:
    explicit Histograms(const InputInfo &info)
        : nbin_(info.nbin()), xlow_(info.bins.xlow), xup_(info.bins.xup) {
        bg_hist_  = TH1D("background", "", nbin_, xlow_, xup_);
        sig_hist_ = TH1D("signal", "", 4.0 * (xup_ - xlow_), xlow_, xup_);
    }
    explicit Histograms(int nbin, double xlow, double xup)
        : nbin_(nbin), xlow_(xlow), xup_(xup) {
        bg_hist_  = TH1D("background", "", nbin_, xlow_, xup_);
        sig_hist_ = TH1D("signal", "", 4.0 * (xup_ - xlow_), xlow_, xup_);
    }
    ~Histograms() {}

    void set_hist(const InputData &data, std::shared_ptr<InputInfo> info);
    double sfnc(double maa) const {
        const int i = 10.0 * (maa - 299.95);
        return y_maa[i];
    }

private:
    TH1D bg_hist_;
    TH1D sig_hist_;

    int nbin_;
    double xlow_, xup_;

    std::vector<double> y_maa;

    void set_bg_hist(const InputData &data, std::shared_ptr<InputInfo> info);
    void set_sig_hist(const InputData &data);
};
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_HISTOGRAMS_H_
