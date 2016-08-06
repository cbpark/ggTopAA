#ifndef GGTOPAA_SRC_TEMPLATES_H_
#define GGTOPAA_SRC_TEMPLATES_H_

#include <map>
#include <string>
#include <vector>

namespace gg2aa {
class Template {
public:
    explicit Template(const std::string fname) : fname_(fname) {}
    ~Template() {}

    std::string file_name() const { return fname_; }
    void set_template(double xlow, double xup, double sqrt_s);
    double sqrt_s() const { return sqrt_s_; }
    double f_maa(double x) const;  // corresponds to sfnc(...) function
                                   // in the original implementation.
    double norm() const;
    double width() const { return xup_ - xlow_; }

    friend double fATL(const Template &t, int nbins, double x, double a1,
                       double a2);

private:
    std::string fname_;
    std::map<double, double> m_aa_;
    double maa_interval_;
    double xlow_, xup_;
    double sqrt_s_;
};

using Templates = std::vector<Template>;
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_TEMPLATES_H_
