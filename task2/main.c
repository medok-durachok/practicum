#include "matrixlib.h"

const int unsigned n = 2;
const int unsigned m = 2;

int main() {
    int command;
    int n1 = n, m1 = m, m2;
    char c[256]; int flag = 0, end_flag = 0;
    double det;
    double * matrix, * additional_matrix;
    printf("Welcome to Matrix Calculator. To read the instruction, press 0.\n");
    scanf("%s", c);
    if(strcmp(c, "0") == 0) {
        instruction();
    }

    printf("\nFirstly enter new matrix with %d rows and %d columns: \n", n, m);
    matrix = input_m(n, m);

    do {
        printf("\n-----------\nCommand #: ");
        do {
            scanf("%s", c);
            flag = check_input(2, c);
        }
        while (flag == 1);
        command = atoi(c);

        switch(command) {
            case 0: 
                instruction();
                break;
            case 1:
                output_m(matrix, n1, m1);
                break;
            case 2:
                matrix = sum_m(matrix, n1, m1);
                break;
            case 3:
                matrix = mul_m(matrix, n1, m1, &m2);
                m1 = m2;
                break;
            case 4:
                matrix = num_m(matrix, n1, m1);
                break;
            case 5:
                det = determinant_gauss(matrix, n1, m1);
                printf("Determinant of matrix is %.2lf", det);
                break;
            case 6:
                end_flag = 1;
                printf("Exit from the Calculator..\n");
                break;
            default: 
                printf("No command found");
                break;
        }    
    } while(end_flag == 0);
    free(matrix);
}