#ifndef GGTOPAA_SRC_HISTOGRAMS_H_
#define GGTOPAA_SRC_HISTOGRAMS_H_

#include <map>
#include <memory>
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

    /**
     *  Fills histogram contents and set the interval of m_aa
     *  in the signal data.
     */
    void set(const InputData &data, std::shared_ptr<InputInfo> info);
    /** Interval of m_aa in the signal data. */
    double delta() const { return maa_interval_; }
    double f_maa(double m) const;  // corresponds to sfnc(...) function
                                   // in the original implementation.
    /** Normalization factor of the signal. */
    double norm_signal() const;

private:
    TH1D bg_hist_;
    TH1D sig_hist_;
    int nbin_;
    double xlow_, xup_;
    double maa_interval_;
    std::map<double, double> m_aa_;

    void set_bg_hist(const InputData &data, std::shared_ptr<InputInfo> info);
    void set_sig_hist(const InputData &data);
};
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_HISTOGRAMS_H_
