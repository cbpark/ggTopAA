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
        while (*f >> x >> y >> z) {
            sig_hist_.Fill(x, y);
            if (x >= xlow_ && x <= xup_) { m_aa_.emplace(x, y); }
        }
    }
}

void Histograms::set(const InputData &data, std::shared_ptr<InputInfo> info) {
    set_bg_hist(data, info);
    set_sig_hist(data);
    maa_interval_ = (xup_ - xlow_) / (m_aa_.size() - 1);
}

double Histograms::f_maa(double m) const {
    if (m < xlow_ || m > xup_) { return 0.0; }
    return m_aa_.lower_bound(m)->second;
}
}  // namespace gg2aa
