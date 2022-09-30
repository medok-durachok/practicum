#ifndef MATRIXLIB_H_INCLUDED
#define MATRIXLIB_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <ctype.h>

int check_input(int err, char * c);

void instruction(void);

double * input_m(int n, int m);

void output_m(double * m_arr, int n, int m);

void rows_cols(int * n, int * m);

double * sum_m(double * m1_arr, int n1, int m1);

double * num_m(double * m_arr, int n, int m);

double * mul_m(double * m1_arr, int n1, int m1, int * k);

double determinant_gauss(double * m_arr, int n, int m);

#endif