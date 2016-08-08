#ifndef GGTOPAA_SRC_HISTOGRAM_H_
#define GGTOPAA_SRC_HISTOGRAM_H_

#include <memory>
#include <ostream>
#include <utility>
#include "TH1D.h"
#include "info.h"
#include "inputdata.h"
#include "utils.h"

namespace gg2aa {
class Histogram {
public:
    Histogram(double bin_size, const Range &r, const char *name,
              const char *title = "")
        : bin_size_(bin_size),
          range_(r),
          num_bins_(static_cast<int>(range_.width() / bin_size_)) {
        hist_ = mkHist(name, title);
    }
    Histogram(const Info &info, const char *name, const char *title = "")
        : bin_size_(info.bin_size),
          range_(Range(info.xlow, info.xup)),
          num_bins_(static_cast<int>(range_.width() / bin_size_)) {
        hist_ = mkHist(name, title);
    }
    ~Histogram() {}

    double bin_size() const { return bin_size_; }
    Range range() const { return range_; }
    int num_bins() const { return num_bins_; }
    std::shared_ptr<TH1D> hist() { return hist_; }

private:
    const double bin_size_;
    const Range range_;
    const int num_bins_;
    std::shared_ptr<TH1D> hist_;

    std::shared_ptr<TH1D> mkHist(const char *name, const char *title = "") {
        return std::make_shared<TH1D>(name, title, num_bins_, range_.low(),
                                      range_.up());
    }
};

class HistObjs {
public:
    explicit HistObjs(const Info &info, double bin_size_signal = 0.25)
        : sig_(std::make_shared<Histogram>(
              bin_size_signal, Range(info.xlow, info.xup), "signal")),
          bg_(std::make_shared<Histogram>(info, "background")) {}
    ~HistObjs() {}

    void fill_hists(const InputData &data, std::shared_ptr<Info> info);
    std::shared_ptr<Histogram> signal() const { return sig_; }
    std::shared_ptr<Histogram> background() const { return bg_; }
    std::shared_ptr<TH1D> pseudo_experiment(const Info &info);

private:
    std::shared_ptr<Histogram> sig_;
    std::shared_ptr<Histogram> bg_;

    void fill_sig_hist(const InputData &data);
    void fill_bg_hist(const InputData &data, std::shared_ptr<Info> info);
};
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_HISTOGRAM_H_
