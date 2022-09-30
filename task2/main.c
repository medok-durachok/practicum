#include "matrixlib.h"

const int unsigned n = 2;
const int unsigned m = 2;

int main() {
    int command;
    int n1 = n, m1 = m, m2;
    char c[256], flag = 0;
    double det;
    double * matrix, * additional_matrix;
    printf("Welcome to Matrix Calculator. To read the instruction, press 0.\n");
    scanf("%d", &command);
    if(command == 0) {
        instruction();
    }

    printf("\nFirstly enter new matrix with %d rows and %d columns: \n", n, m);
    matrix = input_m(n, m);

    while(command != 100) {
        printf("\n-----------\nCommand #: ");
        do{
            scanf("%s", &c);
            flag = check_input(2, &c);
        }
        while (flag == 1);
        command = atoi(&c);

        switch(command){
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
                printf("%.2lf", det);
                break;
            default: 
                printf("error");
                break;
        }    
    }
    free(matrix);
}