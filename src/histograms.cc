#include "histograms.h"
#include <fstream>
#include <memory>
#include "TH1D.h"
#include "inputdata.h"

using std::ifstream;

namespace gg2aa {
void Histograms::set_bg_hist(const InputData &data,
                             std::shared_ptr<Sigma> sigma) {
    double content;
    TH1D hist("hist", "", nbin_, xlow_, xup_);
    int n = 0, n_entries = 0;
    for (const auto &bg : data.background()) {
        if (bg.first == "sigma") { continue; }

        n = n_entries = 0;
        for (const auto &b : bg.second) {
            std::unique_ptr<ifstream> f(new ifstream(b));
            while (*f >> content) {
                hist.Fill(content);
                ++n_entries;
                if (content > xlow_ && content < xup_) { ++n; }
            }
        }
        if (bg.first == "direct") {
            sigma->sig_direct *= static_cast<double>(n) / n_entries;
            hist.Scale(sigma->sig_direct / n_entries);
            bg_hist_.Add(&hist);
        } else if (bg.first == "one-fragment") {
            sigma->sig_one_frag *= static_cast<double>(n) / n_entries;
            hist.Scale(sigma->sig_one_frag / n_entries);
            bg_hist_.Add(&hist);
        } else if (bg.first == "two-fragment") {
            sigma->sig_two_frag *= static_cast<double>(n) / n_entries;
            hist.Scale(sigma->sig_two_frag / n_entries);
            bg_hist_.Add(&hist);
        }
        hist.Reset();
    }
}
}  // namespace gg2aa
