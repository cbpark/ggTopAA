#ifndef GGTOPAA_SRC_TEMPLATES_H_
#define GGTOPAA_SRC_TEMPLATES_H_

#include <map>
#include <memory>
#include <string>
#include <vector>
#include "info.h"
#include "utils.h"

namespace gg2aa {
class Template {
public:
    explicit Template(const std::string &fname) : fname_(fname) {}
    ~Template() {}

    void set_template(const Info &info);
    std::string file_name() const { return fname_; }
    double f_maa(double x) const;
    double norm() const;

private:
    const std::string fname_;
    Range range_;
    double sqrt_s_ = 0.0;
    std::map<double, double> maa_;
    double maa_interval_ = 0.0;
};

using Templates = std::vector<Template>;
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_TEMPLATES_H_
