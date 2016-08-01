#include "histograms.h"
#include <fstream>
#include <memory>
#include "TH1D.h"
#include "inputdata.h"

namespace gg2aa {
void Histograms::set_bg_hist(const InputData &data,
                             std::shared_ptr<InputInfo> info) {
    double content;
    TH1D hist("hist", "", nbin_, xlow_, xup_);
    int n = 0, n_entries = 0;
    for (const auto &bg : data.background()) {
        if (bg.first == "info") { continue; }

        n = n_entries = 0;
        for (const auto &b : bg.second) {
            std::unique_ptr<std::ifstream> f(new std::ifstream(b));
            while (*f >> content) {
                hist.Fill(content);
                ++n_entries;
                if (content > xlow_ && content < xup_) { ++n; }
            }
        }
        if (bg.first == "direct") {
            info->sig_direct *= static_cast<double>(n) / n_entries;
            hist.Scale(info->sig_direct / n_entries);
            bg_hist_.Add(&hist);
        } else if (bg.first == "one-fragment") {
            info->sig_one_frag *= static_cast<double>(n) / n_entries;
            hist.Scale(info->sig_one_frag / n_entries);
            bg_hist_.Add(&hist);
        } else if (bg.first == "two-fragment") {
            info->sig_two_frag *= static_cast<double>(n) / n_entries;
            hist.Scale(info->sig_two_frag / n_entries);
            bg_hist_.Add(&hist);
        }
        hist.Reset();
    }
}

void Histograms::set_sig_hist(const InputData &data) {
    double x, y, z;
    for (const auto &s : data.signal()) {
        std::unique_ptr<std::ifstream> f(new std::ifstream(s));
        while (*f >> x >> y >> z) { sig_hist_.Fill(x, y); }
    }
}

void Histograms::set_hist(const InputData &data, std::shared_ptr<InputInfo> info) {
    set_bg_hist(data, info);
    set_sig_hist(data);
}
}  // namespace gg2aa
