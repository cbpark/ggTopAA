/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_HISTOGRAM_H_
#define SRC_HISTOGRAM_H_

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
    Histogram() = delete;
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
    HistObjs() = delete;
    explicit HistObjs(const Info &info, double bin_size_signal = 0.25)
        : sig_(std::make_shared<Histogram>(
              bin_size_signal, Range(info.xlow, info.xup), "signal")),
          bg_(std::make_shared<Histogram>(info, "background")) {}
    ~HistObjs() {}

    /**
     *  Fill content of the histogram separately from input signal and
     *  background data.
     */
    void fill(const InputData &data, std::shared_ptr<Info> info);

    /** Construct the histogram of the pseudo-experiment data. */
    std::shared_ptr<TH1D> pseudo_experiment(const Info &info) const;

private:
    std::shared_ptr<Histogram> sig_;
    std::shared_ptr<Histogram> bg_;

    void fill_sig(const InputData &data);
    void fill_bg(const InputData &data, std::shared_ptr<Info> info);
};
}  // namespace gg2aa

#endif  // SRC_HISTOGRAM_H_
