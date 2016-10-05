/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#include "histogram.h"
#include <fstream>
#include <memory>
#include "TH1D.h"
#include "TRandom3.h"
#include "info.h"
#include "inputdata.h"
#include "utils.h"

using std::shared_ptr;

namespace gg2aa {
void HistObjs::fill_sig(const InputData &data) {
    double x, y, z;
    for (const auto &s : data.signal()) {
        auto f = std::make_unique<std::ifstream>(s);
        while (*f >> x >> y >> z) {
            // This causes 0.05 GeV shift in the m_{aa} distribution.
            sig_->hist()->Fill(x + 0.01, y);
        }
        f->close();
    }
}

void HistObjs::fill_bg(const InputData &data, shared_ptr<Info> info) {
    const Range r(info->xlow, info->xup);
    auto hist = std::make_unique<TH1D>("hist", "", r.width() / info->bin_size,
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
                if (content > r.low() && content < r.up()) { ++n; }
            }
            f->close();
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

void HistObjs::fill(const InputData &data, shared_ptr<Info> info) {
    fill_sig(data);
    fill_bg(data, info);
}

shared_ptr<TH1D> HistObjs::pseudo_experiment(const Info &info,
                                             const bool set_seed) const {
    const auto r = bg_->range();
    auto h_pd = std::make_shared<TH1D>("hPD", "Pseudo data", bg_->num_bins(),
                                       r.low(), r.up());
    if (set_seed) { gRandom->SetSeed(0); }
    // gRandom->Print();
    h_pd->FillRandom(bg_->hist().get(), info.n_bg());
    h_pd->FillRandom(sig_->hist().get(), info.n_sig());
    // const int n_bg = info.n_bg();
    // for (int i = 0; i != n_bg; ++i) { h_pd->Fill(bg_->hist()->GetRandom()); }
    // const int n_sig = info.n_sig();
    // for (int i = 0; i != n_sig; ++i) { h_pd->Fill(sig_->hist()->GetRandom());
    // }
    // h_pd->SetMinimum(0.0);
    return h_pd;
}
}  // namespace gg2aa
