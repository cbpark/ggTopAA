#ifndef GGTOPAA_SRC_INPUTDATA_H_
#define GGTOPAA_SRC_INPUTDATA_H_

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace gg2aa {
using FileName   = std::string;
using InputFiles = std::vector<FileName>;
using Backgrounds = std::unordered_map<std::string, InputFiles>;

enum class InputStatus { NONE, SIGMA, DIRECT, FRAGMENT1, FRAGMENT2, SIGNAL };

class InputData {
public:
    InputData() {}
    ~InputData() {}

    InputFiles signal() const { return signal_; }
    void add_signal(FileName fname) { signal_.push_back(fname); }
    std::string show_signal() const;

    Backgrounds background() const { return background_; }
    InputFiles background(std::string k) const { return background_.at(k); }
    void add_background(std::string k, const FileName &fname) {
        background_[k].push_back(fname);
    }
    std::string show_background(std::string k) const;

    std::string show() const;

    InputStatus get_status() const { return status_; }
    void set_status(InputStatus s) { status_ = s; }
    void reset_status() { status_ = InputStatus::NONE; }
    std::string show_status() const;

    std::pair<int, InputFiles> check_input() const;

private:
    InputFiles signal_;
    Backgrounds background_;
    InputStatus status_ = InputStatus::NONE;
};

struct Sigma {
    double rs, lum, eff, kg;
    double sig_direct, sig_one_frag, sig_two_frag;
    double bin_size, minbin, maxbin;
    double a1in, a2in, bin;
    int status = 0;

    std::string show() const;
    int nbin() const { return static_cast<int>((maxbin - minbin) / bin_size); }
    std::string show_sig() const;
    std::string show_bg_summary() const;
};
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_INPUTDATA_H_
