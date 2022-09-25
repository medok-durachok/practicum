#include "matrix_lib.h"

const int unsigned n = 3;
const int unsigned m = 3;

double * input_m(){
    double * arr;
    arr = malloc(n * m * sizeof(double));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            scanf("%lf", &arr[i * n + j]);
        }
    }
    return arr;
}

void output_m(double * m_arr){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            printf("%.2lf\t", m_arr[i * n + j]);
        }
        printf("\n");
    }
}