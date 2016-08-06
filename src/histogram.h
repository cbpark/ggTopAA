#ifndef GGTOPAA_SRC_HISTOGRAM_H_
#define GGTOPAA_SRC_HISTOGRAM_H_

#include <ostream>
#include <utility>

namespace gg2aa {
class Histogram {
public:
    Histogram() {}
    Histogram(double bin_size, double xlow, double xup)
        : bin_size_(bin_size), xlow_(xlow), xup_(xup) {
        num_bins_ = static_cast<int>((xup_ - xlow_) / bin_size_);
    }
    ~Histogram() {}

    void show_info(std::ostream *out) const;
    int num_bins() const { return num_bins_; }
    double bin_size() const { return bin_size_; }
    std::pair<double, double> bound() const {
        return std::make_pair(xlow_, xup_);
    }
    double fullwidth() const { return xup_ - xlow_; }
    bool in_range(double x) const { return x >= xlow_ && x <= xup_; }

private:
    double bin_size_;
    int num_bins_;
    double xlow_, xup_;
};
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_HISTOGRAM_H_
