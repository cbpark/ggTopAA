/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_CONTOURS_H_
#define SRC_CONTOURS_H_

#include <array>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "TGraph.h"
#include "TGraph2D.h"
#include "TH2D.h"
#include "fit.h"

namespace gg2aa {
struct ContAxisInfo {
    ContAxisInfo(const std::string &t, const std::array<double, 2> &r,
                 const int n)
        : title(t), range(r), ndiv(n) {}
    ~ContAxisInfo() {}

    std::string title;
    std::array<double, 2> range;
    int ndiv;
};

class Contour {
public:
    virtual ~Contour() {}
    virtual std::shared_ptr<TGraph2D> graph() = 0;

    TH2D *hist() { return graph()->GetHistogram(); }
    std::array<double, 2> min_point() const { return min_point_; }
    double min_value() const { return min_value_; }

    double operator()(const double *x) {
        return graph()->Interpolate(x[0], x[1]);
    }

protected:
    std::array<double, 2> min_point_;
    double min_value_;

    void set_min_point(const double x, const double y, const double step,
                       std::function<double(const double *)> func);
    void set_hist(const ContAxisInfo &x_axis, const ContAxisInfo &y_axis,
                  std::shared_ptr<TGraph2D> graph);
};

std::shared_ptr<TGraph> graphMinPoint(const Contour &cont);

void set_cont_levels(const double oneSigma, const double twoSigma,
                     Contour *cont);

class MassWidthCont : public Contour {
public:
    MassWidthCont() = delete;
    explicit MassWidthCont(const std::vector<FitResult> &fres)
        : graph_(mkGraph2D(fres)) {
        set_min_point(173, 1.5, 0.0001, *this);

        const ContAxisInfo x_axis("m_{t} (GeV)", {{172, 174}}, 505);
        const ContAxisInfo y_axis("#Gamma_{t} (GeV)", {{0.5, 3.5}}, 504);
        set_hist(x_axis, y_axis, graph_);
    }
    ~MassWidthCont() {}

    std::shared_ptr<TGraph2D> graph() { return graph_; }

private:
    std::shared_ptr<TGraph2D> graph_;
    std::shared_ptr<TGraph2D> mkGraph2D(const std::vector<FitResult> &fres);
};

std::pair<double, double> chi2Interval12(const int fit_choice);
}  // namespace gg2aa

#endif  // SRC_CONTOURS_H_
