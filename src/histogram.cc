#include "histogram.h"
#include <fstream>
#include <memory>
#include "TH1D.h"
#include "info.h"
#include "inputdata.h"
#include "utils.h"

namespace gg2aa {
void HistObjs::fill_sig_hist(const InputData &data) {
    double x, y, z;
    for (const auto &s : data.signal()) {
        std::unique_ptr<std::ifstream> f(new std::ifstream(s));
        while (*f >> x >> y >> z) { sig_->hist()->Fill(x, y); }
    }
}

void HistObjs::fill_bg_hist(const InputData &data, std::shared_ptr<Info> info) {
    const Range r(info->xlow, info->xup);
    auto hist = std::make_shared<TH1D>(
        TH1D("hist", "", r.width() / info->bin_size, r.low(), r.up()));
    double content;
    int n = 0, n_entries = 0;
    for (const auto &bg : data.background()) {
        if (bg.first == "info") { continue; }

        for (const auto &b : bg.second) {
            std::unique_ptr<std::ifstream> f(new std::ifstream(b));
            while (*f >> content) {
                hist->Fill(content);
                ++n_entries;
                if (r.includes(content)) { ++n; }
            }
        }
        if (bg.first == "direct") {
            info->sig_direct *= static_cast<double>(n) / n_entries;
            hist->Scale(info->sig_direct / n_entries);
            bg_->hist()->Add(&*hist);
        } else if (bg.first == "one-fragment") {
            info->sig_one_frag *= static_cast<double>(n) / n_entries;
            hist->Scale(info->sig_one_frag / n_entries);
            bg_->hist()->Add(&*hist);
        } else if (bg.first == "two-fragment") {
            info->sig_two_frag *= static_cast<double>(n) / n_entries;
            hist->Scale(info->sig_two_frag / n_entries);
            bg_->hist()->Add(&*hist);
        }
        n = n_entries = 0;
        hist->Reset();
    }
}

void HistObjs::fill_hists(const InputData &data, std::shared_ptr<Info> info) {
    fill_sig_hist(data);
    fill_bg_hist(data, info);
}

std::shared_ptr<TH1D> HistObjs::pseudo_experiment(const Info& info) {
    auto r = bg_->range();
    auto h_pd = std::make_shared<TH1D>(
        TH1D("hPD", "Pseudo data", bg_->num_bins(), r.low(), r.up()));

    // h_pd->Add(&*(sig_->hist()));
    // h_pd->Add(&*(bg_->hist()));
    const int n_sig = info.n_sig();
    h_pd->FillRandom(sig_->hist().get(), n_sig);
    // for (int i = 0; i != n_sig; ++i) {
    //     h_pd->Fill(sig_->hist()->GetRandom(), 1.0);
    // }
    const int n_bg = info.n_bg();
    h_pd->FillRandom(bg_->hist().get(), n_bg);
    // for (int i = 0; i != n_bg; ++i) {
    //     h_pd->Fill(bg_->hist()->GetRandom(), 1.0);
    // }

    // h_pd->SetMinimum(0.0);
    return h_pd;
}
}  // namespace gg2aa
