#ifndef GGTOPAA_SRC_UTILS_H_
#define GGTOPAA_SRC_UTILS_H_

#include <string>
#include <utility>
#include <vector>

namespace gg2aa {
class Range {
public:
    Range() {}
    Range(double x1, double x2) {
        if (x1 > x2) {
            low_ = x2;
            up_ = x1;
        } else {
            low_ = x1;
            up_ = x2;
        }
    }
    ~Range() {}

    double low() const { return low_; }
    double up() const { return up_; }
    double width() const { return up_ - low_; }
    bool includes(double x) const { return x >= low_ && x <= up_; }

private:
    double low_, up_;
};

std::vector<std::string> split(const std::string &str, char c);

std::pair<double, double> getMassWidth(const std::string &fname);
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_UTILS_H_
