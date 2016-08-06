#include "histograms.h"
#include <cmath>
#include <fstream>
#include <functional>
#include <memory>
#include "Math/SpecFuncMathMore.h"
#include "TH1D.h"
#include "inputdata.h"

namespace gg2aa {
void Histograms::set_bg_hist(const InputData &data,
                             std::shared_ptr<InputInfo> info) {
    const auto bound = bg_hist_bin_.hist_bound();
    TH1D hist("hist", "", bg_hist_bin_.num_bins(), bound.first, bound.second);
    double content;
    int n = 0, n_entries = 0;
    for (const auto &bg : data.background()) {
        if (bg.first == "info") { continue; }

        n = n_entries = 0;
        for (const auto &b : bg.second) {
            std::unique_ptr<std::ifstream> f(new std::ifstream(b));
            while (*f >> content) {
                hist.Fill(content);
                ++n_entries;
                if (bg_hist_bin_.in_range(content)) { ++n; }
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
            if (sig_hist_bin_.in_range(x)) { m_aa_.emplace(x, y); }
        }
    }
}

void Histograms::set(const InputData &data, std::shared_ptr<InputInfo> info) {
    set_bg_hist(data, info);
    set_sig_hist(data);
    sqrt_s_       = info->rs;
    maa_interval_ = sig_hist_bin_.width() / (m_aa_.size() - 1);
}

double Histograms::f_maa(double m) const {
    if (!sig_hist_bin_.in_range(m)) { return 0.0; }
    return m_aa_.lower_bound(m)->second;
}

double simpson(std::function<double(double)> func, double xlow, double xup,
               double delta) {
    int n = (xup - xlow) / delta;
    if (n % 2 == 1) {
        --n;
        xup -= delta;
    }
    n /= 2;
    double norm = func(xlow) + 4.0 * func(xup - delta) + func(xup);
    for (int i = 0; i != n; ++i) {
        norm += 4.0 * func(xlow + (2 * i + 1) * delta) +
                2.0 * func(xlow + (2 * i + 2) * delta);
    }
    return norm * delta / 3.0;
}

double Histograms::norm_signal() const {
    const auto bound = sig_hist_bin_.hist_bound();
    return simpson(std::bind(&Histograms::f_maa, this, std::placeholders::_1),
                   bound.first, bound.second, maa_interval_);
}

double poly_cbrt(double x, double a1, double a2) {
    return std::pow((1.0 - std::cbrt(x)), a1) * std::pow(x, a2);
}

double fATL(const Histograms &h, double x, double a1, double a2) {
    using std::pow;

    auto bound  = h.sig_hist_bin_.hist_bound();
    double xmax = bound.second / h.sqrt_s_, xmin = bound.first / h.sqrt_s_;
    double sATL = 0.0;
    if (std::fabs(1.0 + a2) < 1.0e-3) {
        double delta = (xmax - xmin) / h.sig_hist_bin_.num_bins();
        for (int i = 0; i <= h.sig_hist_bin_.num_bins(); ++i) {
            sATL += poly_cbrt(xmin + delta * i, a1, a2);
        }
        sATL *= delta;
    } else {
        double a = -a1, b = 3.0 * (1.0 + a2), c = 4.0 + 3.0 * a2;
        double cbrt_xmax = std::cbrt(xmax), cbrt_xmin = std::cbrt(xmin);
        double pf1 = 1.0, pf2 = 1.0;
        if (a < -10.0 || b < -10.0) {
            pf1 = pow(1.0 - cbrt_xmax, c - b - a);
            pf2 = pow(1.0 - cbrt_xmin, c - b - a);
            a   = c - a;
            b   = c - b;
        }
        double hyp1 = pf1 * ROOT::Math::hyperg(a, b, c, cbrt_xmax);
        double hyp2 = pf2 * ROOT::Math::hyperg(a, b, c, cbrt_xmin);

        sATL = (pow(xmax, 1.0 + a2) * hyp1 - pow(xmin, 1.0 + a2) * hyp2) /
               (1.0 + a2);
    }
    return poly_cbrt(x, a1, a2) / sATL;
}
}  // namespace gg2aa
