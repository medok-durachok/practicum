#ifndef MATRIXLIB_H_INCLUDED
#define MATRIXLIB_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

void instruction(void);

double * input_m(void);

void output_m(double * m_arr);

double * sum_m(double * m1_arr, double * m2_arr, int n1, int m1, int n2, int m2);

double * num_m(double * m_arr);

double * mul_m(double * m1_arr, double * m2_arr, int n1, int m1, int m2, int k2);

void determinant_gauss(double * m_arr, int nn, int mm);

#endif // MATRIX_LIB_H_INCLUDED