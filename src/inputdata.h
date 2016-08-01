#ifndef GGTOPAA_SRC_INPUTDATA_H_
#define GGTOPAA_SRC_INPUTDATA_H_

#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace gg2aa {
using FileName    = std::string;
using InputFiles  = std::vector<FileName>;
using Backgrounds = std::unordered_map<std::string, InputFiles>;

enum class InputStatus { NONE, INFO, DIRECT, FRAGMENT1, FRAGMENT2, SIGNAL };

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

    void show(std::ostream *out) const;

    InputStatus get_status() const { return status_; }
    void set_status(InputStatus s) { status_ = s; }
    void reset_status() { status_ = InputStatus::NONE; }
    void show_status(std::ostream *out) const;

    std::pair<int, InputFiles> check_input() const;

private:
    InputFiles signal_;
    Backgrounds background_;
    InputStatus status_ = InputStatus::NONE;
};

struct HistBin {
    double size;
    double xlow;
    double xup;

    void show(std::ostream *out) const;
    int num_bin() const { return static_cast<int>((xup - xlow) / size); }
};

struct InputInfo {
    double rs, lum, eff, kg;
    double sig_direct, sig_one_frag, sig_two_frag;
    HistBin bins;
    double a1in, a2in, bin;
    int status = 0;

    void show(std::ostream *out) const;
    int nbin() const { return bins.num_bin(); }
    void show_bg_summary(std::ostream *out) const;

    double sig_bg() const { return sig_direct + sig_one_frag + sig_two_frag; }
    int n_bg() const { return static_cast<int>(sig_bg() * lum * eff * 1.0e3); }
    int n_sig() const { return static_cast<int>(n_bg() * kg / (1.0 - kg)); }
    int nev() const { return n_bg() + n_sig(); }
};
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_INPUTDATA_H_
