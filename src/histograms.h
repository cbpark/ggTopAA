#ifndef GGTOPAA_SRC_HISTOGRAMS_H_
#define GGTOPAA_SRC_HISTOGRAMS_H_

#include <map>
#include <memory>
#include "TH1D.h"
#include "inputdata.h"

namespace gg2aa {
class Histograms {
public:
    explicit Histograms(const InputInfo &info) : sqrt_s_(info.rs) {
        const auto bound = info.bins.hist_bound();

        sig_hist_bin_ = HistBin(0.25, bound);
        sig_hist_     = sig_hist_bin_.mkHist("signal");

        bg_hist_bin_ = HistBin(info.bins.bin_size(), bound);
        bg_hist_     = bg_hist_bin_.mkHist("background");
    }
    ~Histograms() {}

    /**
     *  Fills histogram contents and set the interval of m_aa
     *  in the signal data.
     */
    void set(const InputData &data, std::shared_ptr<InputInfo> info);
    /** Number of total events */
    int nevent() const { return n_event_; }
    /** The center of mass energy, sqrt(s). */
    double sqrt_s() const { return sqrt_s_; }
    /** Interval of m_aa in the signal data. */
    double delta() const { return maa_interval_; }
    double f_maa(double m) const;  // corresponds to sfnc(...) function
                                   // in the original implementation.
    /** Normalization factor of the signal. */
    double norm_signal() const;
    /** Number of bins of the background histogram. */
    int bg_num_bins() const { return bg_hist_bin_.num_bins(); }
    /** (xmax - xmin) of histogram. */
    double hist_width() const { return sig_hist_bin_.width(); }

    friend double fATL(const Histograms &h, double x, double a1, double a2);

private:
    TH1D bg_hist_;
    TH1D sig_hist_;
    HistBin sig_hist_bin_;
    HistBin bg_hist_bin_;
    int n_event_;
    /// proton-proton collision energy.
    const double sqrt_s_;

    double maa_interval_;
    std::map<double, double> m_aa_;

    void set_bg_hist(const InputData &data, std::shared_ptr<InputInfo> info);
    void set_sig_hist(const InputData &data);
};
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_HISTOGRAMS_H_
