#include "histogram.h"
#include <fstream>
#include <memory>
#include "inputdata.h"
#include "inputinfo.h"
#include "utils.h"

namespace gg2aa {
void HistObjs::fill_sig_hist(const InputData &data) {
    double x, y, z;
    for (const auto &s : data.signal()) {
        std::unique_ptr<std::ifstream> f(new std::ifstream(s));
        while (*f >> x >> y >> z) { sig_.hist().Fill(x, y); }
    }
}

void HistObjs::fill_bg_hist(const InputData &data,
                            std::shared_ptr<InputInfo> info) {
    const Range r(info->xlow, info->xup);
    TH1D hist("hist", "", r.width() / info->bin_size, r.low(), r.up());
    double content;
    int n = 0, n_entries = 0;
    for (const auto &bg : data.background()) {
        if (bg.first == "info") { continue; }

        for (const auto &b : bg.second) {
            std::unique_ptr<std::ifstream> f(new std::ifstream(b));
            while (*f >> content) {
                hist.Fill(content);
                ++n_entries;
                if (r.includes(content)) { ++n; }
            }
        }
        if (bg.first == "direct") {
            info->sig_direct *= static_cast<double>(n) / n_entries;
            hist.Scale(info->sig_direct / n_entries);
            bg_.hist().Add(&hist);
        } else if (bg.first == "one-fragment") {
            info->sig_one_frag *= static_cast<double>(n) / n_entries;
            hist.Scale(info->sig_one_frag / n_entries);
            bg_.hist().Add(&hist);
        } else if (bg.first == "two-fragment") {
            info->sig_two_frag *= static_cast<double>(n) / n_entries;
            hist.Scale(info->sig_two_frag / n_entries);
            bg_.hist().Add(&hist);
        }
        n = n_entries = 0;
        hist.Reset();
    }
}

void HistObjs::fill_hists(const InputData &data,
                          std::shared_ptr<InputInfo> info) {
    fill_sig_hist(data);
    fill_bg_hist(data, info);
}
}  // namespace gg2aa
