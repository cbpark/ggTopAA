#include "histogram.h"
#include <fstream>
#include <memory>
#include "TH1D.h"
#include "info.h"
#include "inputdata.h"
#include "utils.h"

using std::shared_ptr;

namespace gg2aa {
void HistObjs::fill_sig_hist(const InputData &data) {
    double x, y, z;
    for (const auto &s : data.signal()) {
        auto f = std::make_unique<std::ifstream>(s);
        while (*f >> x >> y >> z) { sig_->hist()->Fill(x, y); }
    }
}

void HistObjs::fill_bg_hist(const InputData &data, shared_ptr<Info> info) {
    const Range r(info->xlow, info->xup);
    auto hist = std::make_shared<TH1D>("hist", "", r.width() / info->bin_size,
                                       r.low(), r.up());
    double content;
    int n = 0, n_entries = 0;
    for (const auto &bg : data.background()) {
        if (bg.first == "info") { continue; }

        for (const auto &b : bg.second) {
            auto f = std::make_unique<std::ifstream>(b);
            while (*f >> content) {
                hist->Fill(content);
                ++n_entries;
                if (r.includes(content)) { ++n; }
            }
        }
        if (bg.first == "direct") {
            info->sig_direct *= static_cast<double>(n) / n_entries;
            hist->Scale(info->sig_direct / n_entries);
            bg_->hist()->Add(hist.get());
        } else if (bg.first == "one-fragment") {
            info->sig_one_frag *= static_cast<double>(n) / n_entries;
            hist->Scale(info->sig_one_frag / n_entries);
            bg_->hist()->Add(hist.get());
        } else if (bg.first == "two-fragment") {
            info->sig_two_frag *= static_cast<double>(n) / n_entries;
            hist->Scale(info->sig_two_frag / n_entries);
            bg_->hist()->Add(hist.get());
        }
        n = n_entries = 0;
        hist->Reset();
    }
}

void HistObjs::fill_hists(const InputData &data, shared_ptr<Info> info) {
    fill_sig_hist(data);
    fill_bg_hist(data, info);
}

shared_ptr<TH1D> HistObjs::pseudo_experiment(const Info &info) const {
    const auto r = bg_->range();
    auto h_pd = std::make_shared<TH1D>("hPD", "Pseudo data", bg_->num_bins(),
                                       r.low(), r.up());
    h_pd->FillRandom(sig_->hist().get(), info.n_sig());
    h_pd->FillRandom(bg_->hist().get(), info.n_bg());
    // h_pd->SetMinimum(0.0);
    return h_pd;
}
}  // namespace gg2aa
