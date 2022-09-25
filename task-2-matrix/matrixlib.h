#ifndef MATRIXLIB_H
#define MATRIXLIB_H
#include <stdio.h>
#include <stdlib.h>

void instruction();

double * input_m();

void output_m(double * m_arr);

double * sum_m(double * m1_arr, double * m2_arr);

double * num_m(double * m_arr, double k);

double * mul_m(double * a, double * b, int nn, int mm, int kk);

#endif // MATRIX_LIB_H_INCLUDED
