#ifndef GGTOPAA_SRC_INPUTDATA_H_
#define GGTOPAA_SRC_INPUTDATA_H_

#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "TH1D.h"
#include "templates.h"

namespace gg2aa {
using FileName    = std::string;
using InputFiles  = std::vector<FileName>;
using Backgrounds = std::unordered_map<std::string, InputFiles>;

enum class InputStatus {
    NONE,
    INFO,
    DIRECT,
    FRAGMENT1,
    FRAGMENT2,
    SIGNAL,
    TEMPLATE
};

class InputData {
public:
    InputData() {}
    ~InputData() {}

    InputFiles signal() const { return signal_; }
    void add_signal(FileName fname) { signal_.push_back(fname); }
    void show_signal(std::ostream *out) const;

    Backgrounds background() const { return background_; }
    InputFiles background(std::string k) const { return background_.at(k); }
    void add_background(std::string k, const FileName &fname) {
        background_[k].push_back(fname);
    }
    void show_background(std::string k, std::ostream *out) const;

    Templates templates() const { return templates_; }
    void add_templates(FileName fname)  { templates_.push_back(Template(fname)); }
    void show_templates(std::ostream *out) const;

    void show(std::ostream *out) const;

    InputStatus get_status() const { return status_; }
    void set_status(InputStatus s) { status_ = s; }
    void reset_status() { status_ = InputStatus::NONE; }
    void show_status(std::ostream *out) const;

    std::pair<int, InputFiles> check_input() const;

private:
    InputFiles signal_;
    Backgrounds background_;
    Templates templates_;
    InputStatus status_ = InputStatus::NONE;
};

class HistBin {
public:
    HistBin() {}
    HistBin(double bin_size, std::pair<double, double> bound)
        : bin_size_(bin_size), xlow_(bound.first), xup_(bound.second) {
        num_bins_ = static_cast<int>((xup_ - xlow_) / bin_size_);
    }
    explicit HistBin(double bin_size, double xlow, double xup)
        : bin_size_(bin_size), xlow_(xlow), xup_(xup) {
        num_bins_ = static_cast<int>((xup_ - xlow_) / bin_size_);
    }
    ~HistBin() {}

    void show(std::ostream *out) const;
    int num_bins() const { return num_bins_; }
    double bin_size() const { return bin_size_; }
    std::pair<double, double> hist_bound() const {
        return std::make_pair(xlow_, xup_);
    }
    /// The width of the histogram: xup - xlow
    double width() const { return xup_ - xlow_; }
    bool in_range(double x) const { return x >= xlow_ && x <= xup_; }
    TH1D mkHist(const char *name, const char *title = "") {
        return TH1D(name, title, num_bins_, xlow_, xup_);
    }

private:
    /// Bin size.
    double bin_size_;
    /// Number of bins.
    int num_bins_;
    double xlow_;
    double xup_;
};

struct InputInfo {
    double rs, lum, eff, kg;
    double sig_direct, sig_one_frag, sig_two_frag;
    HistBin bins;
    double a1in, a2in, bin;
    int status = 0;

    void show(std::ostream *out) const;
    int nbin() const { return bins.num_bins(); }
    void show_bg_summary(std::ostream *out) const;

    double sig_bg() const { return sig_direct + sig_one_frag + sig_two_frag; }
    int n_bg() const { return static_cast<int>(sig_bg() * lum * eff * 1.0e3); }
    int n_sig() const { return static_cast<int>(n_bg() * kg / (1.0 - kg)); }
    int nev() const { return n_bg() + n_sig(); }
};
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_INPUTDATA_H_
