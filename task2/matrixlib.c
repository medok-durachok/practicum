#include "matrixlib.h"

void instruction(void) {
    printf("-----------\n%s\n-----------", "List of avaliable commands:");
    printf("\'out\' - output the matrix\n");
    printf("\'+\' - addition of 2 matrices\n");
    printf("\'*\' - multiplication of 2 matrices\n");
    printf("\'num\' - multiplying a matrix by a number\n");
    printf("\'det\' - calculation of the determinant of the matrix\n");
    printf("-----------\n");
}

double * input_m(int n, int m) {
    double * arr;
    arr = malloc(n * m * sizeof(double));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            scanf("%lf", &arr[i * n + j]);
        }
    }
    return arr;
}

void output_m(double * m_arr, int n, int m) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            printf("%.2lf\t", m_arr[i * n + j]);
        }
        printf("\n");
    }
}

double * sum_m(double * m1_arr, double * m2_arr, int n1, int m1, int n2, int m2) {
    if ((n1 != n2) || (m1 != m2)) {
        printf("\n! The sizes of the matrices do not match !");
        return NULL;
    }

    int n = n1;
    int m = m1;

    double * sum_arr;
    sum_arr = malloc(n * m * sizeof(double));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            sum_arr[i * n + j] = m1_arr[i * n + j] + m2_arr[i * n + j];
        }
    }
    return sum_arr;
}

double * num_m(double * m_arr, int n, int m) {
    double * num_arr;
    num_arr = malloc(n * m * sizeof(double));

    double k;
    printf("\nEnter number: ");
    scanf("%lf", &k);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++){
            num_arr[i * n + j] = m_arr[i * n + j] * k;
        }
    }
    return num_arr;
}

double * mul_m(double * m1_arr, double * m2_arr, int n1, int m1, int m2, int k2) {
    if(m1 != m2) {
        printf("\n! The sizes of the matrices do not match !");
        return NULL;
    }
    int mm = m1;

    double * m_arr;
    m_arr = malloc(n1 * k2 * sizeof(double));
    int sum = 0;
    for(int i = 0; i < n1; i++) {
        for(int k = 0; k < k2; k++) {
            for(int j = 0; j < mm; j++) {
                sum += m1_arr[i * n1 + j] * m2_arr[j * mm + k];
            }
            m_arr[i * n1 + k] = sum;
            sum = 0;
        }
    }

    return m_arr;
}

void determinant_gauss(double * m_arr, int n, int m) {
    if (m != n) {
        printf("\n! the matrix is not square !");
        exit;
    }

    int minim = INT_MAX, i = 0 , j = 0, l, sq = n;
    double k = 0, det = 1;

    while(i < sq && j < sq) {
        for(int ii = i; ii < sq; ii++) {
            if(abs(m_arr[ii * sq + j]) < minim) {
                minim = abs(m_arr[ii * sq + j]);
                l = ii;
            }
        }
        minim = INT_MAX;

        int temp;
        for(int jj = 0; jj < sq; jj++) {
            temp = m_arr[i * sq + jj];
            m_arr[i * sq + jj] = m_arr[l * sq + jj];
            m_arr[l * sq + jj] = temp;
        }

        for(int ii = i + 1; ii < sq; ii++) {
            k = m_arr[ii * sq + j] / m_arr[i * sq + j];
            m_arr[ii * sq + j] = 0;

            for(int jj = j + 1; jj < sq; jj++) {
                m_arr[ii * sq + jj] -= k * m_arr[i * sq + jj];
            }
        }
        i++;
        j++;
    }

    for(int ii = 0; ii < 3; ii++) {
        det *= m_arr[ii * sq + ii];
    }

    printf("%.2lf\n", det);
}
