#include "matrixlib.h"

const int unsigned n = 3;
const int unsigned m = 3;

void instruction(){
    printf("---------\n%s\n-----------", "List of avaliable commands:");
    printf("\'+\' — addition of 2 matrices\n");
    printf("\'*\' — multiplication of 2 matrices\n");
    printf("\'num\' — multiplying a matrix by a number\n");
    printf("\'det\' — calculation of the determinant of the matrix\n");
    printf("-----------\n");
}

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

double * sum_m(double * m1_arr, double * m2_arr){
    double * sum_arr;
    sum_arr = malloc(n * m * sizeof(double));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            sum_arr[i * n + j] = m1_arr[i * n + j] + m2_arr[i * n + j];
        }
    }
    return sum_arr;
}

double * num_m(double * m_arr, double k){
    double * num_arr;
    num_arr = malloc(n * m * sizeof(double));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            num_arr[i * n + j] = m_arr[i * n + j] * k;
        }
    }
    return num_arr;
}

double * mul_m(double * a, double * b, int nn, int mm, int kk){
    double * c;
    c = malloc(nn * kk * sizeof(double));
    int sum = 0;
    for(int i = 0; i < nn; i++){
        for(int k = 0; k < kk; k++){
            for(int j = 0; j < mm; j++){
                sum += a[i * nn + j] * b[j * mm + k];
            }
            c[i * nn + k] = sum;
            sum = 0;
        }
    }

    return c;
}