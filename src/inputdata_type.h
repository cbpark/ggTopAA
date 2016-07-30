#ifndef GGTOPAA_SRC_INPUTDATA_TYPE_H_
#define GGTOPAA_SRC_INPUTDATA_TYPE_H_

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace gg2aa {
using FileName   = std::string;
using InputFiles = std::vector<FileName>;

enum class InputStatus { NONE, SIGMA, DIRECT, FRAGMENT, SIGNAL };

class InputData {
public:
    InputData() {}
    ~InputData() {}

    InputFiles signal() const { return signal_; }
    void add_signal(FileName fname) { signal_.push_back(fname); }
    std::string show_signal() const;

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
    std::unordered_map<std::string, InputFiles> background_;
    InputStatus status_ = InputStatus::NONE;
};
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_INPUTDATA_TYPE_H_
