#ifndef MATRIXLIB_H_INCLUDED
#define MATRIXLIB_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

void instruction();

double * input_m();

void output_m(double * m_arr);

double * sum_m(double * m1_arr, double * m2_arr);

double * num_m(double * m_arr, double k);

double * mul_m(double * a, double * b, int nn, int mm, int kk);

void determinant_gauss(double * m_arr, int nn, int mm);

#endif // MATRIX_LIB_H_INCLUDED