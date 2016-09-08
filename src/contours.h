/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_CONTOURS_H_
#define SRC_CONTOURS_H_

#include <functional>
#include <memory>
#include <utility>
#include <vector>
#include "TGraph2D.h"
#include "fit.h"

namespace gg2aa {
/**
 *  The result of fitting is stored in the function using
 *  TGraph2D::Interpolate of ROOT.
 */
class MassWidthCont {
public:
    MassWidthCont() = delete;
    explicit MassWidthCont(const std::vector<FitResult> &fres)
        : graph_(mkGraph2D(fres)) {
        init_hist();
    }
    ~MassWidthCont() {}

    std::shared_ptr<TGraph2D> graph() { return graph_; }
    double operator()(const double *x) const {
        return graph_->Interpolate(x[0], x[1]);
    }

private:
    std::shared_ptr<TGraph2D> graph_;
    std::shared_ptr<TGraph2D> mkGraph2D(const std::vector<FitResult> &fres);
    void init_hist();
};

std::pair<std::array<double, 2>, double> minPoint(
    std::function<double(const double *)> func);
}  // namespace gg2aa

#endif  // SRC_CONTOURS_H_
