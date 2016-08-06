#ifndef GGTOPAA_SRC_TEMPLATES_H_
#define GGTOPAA_SRC_TEMPLATES_H_

#include <string>
#include <vector>

namespace gg2aa {
class Template {
public:
    explicit Template(const std::string fname) : fname_(fname) {}
    ~Template() {}

    std::string file_name() const { return fname_; }

private:
    std::string fname_;
};

using Templates = std::vector<Template>;
}  // namespace gg2aa

#endif  // GGTOPAA_SRC_TEMPLATES_H_
