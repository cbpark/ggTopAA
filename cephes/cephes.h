/*
 *  Copyright (C) 2016 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of ggTopAA, which is released under the GNU General Public
 *  License. See file LICENSE in the top directory of this project or go to
 *  <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef CEPHES_CEPHES_HYP2F1_H_
#define CEPHES_CEPHES_HYP2F1_H_

extern "C" {
double pow(double x, double y);

double cbrt(double x);

double hyp2f1(double a, double b, double c, double x);
}

#endif  // CEPHES_CEPHES_HYP2F1_H_
