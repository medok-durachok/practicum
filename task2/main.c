#include "matrixlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int unsigned n = 3;
const int unsigned m = 3;

int main() {
    char command[6];
    int n1, m1, n2, m2;
    double * matrix, * additional_matrix;
    printf("Welcome to Matrix Calculator. To read the instruction, press 0.\n");
    scanf("%s", command);
    if(strcmp(command, "0") == 0) {
        instruction();
    }

    printf("\nFirstly enter new matrix: %d rows and %d columns: ", n, m);
    matrix = input_m(n, m);

    while(strcmp(command, "exit") != 0) {
        printf("-----------\nCommand: ");
        scanf("%s", command);

        if (strcmp(command, "out") == 0) {
            output_m(matrix, n, m);
        }
        if (strcmp(command, "+") == 0) {
            printf("\nEnter the number of rows: ");
            scanf("%d", &n2);
            printf("and the number of columns: ");
            scanf("%d", &m2);
            additional_matrix = input_m(n2, m2);
            matrix = sum_m(matrix, additional_matrix, n1, m1, n2 ,m2);
        }
        if (strcmp(command, "*") == 0) {
            printf("\nEnter the number of rows: ");
            scanf("%d", &n2);
            printf("and the number of columns: ");
            scanf("%d", &m2);
            additional_matrix = input_m(n2, m2);
            matrix = mul_m(matrix, additional_matrix, n1, m1, n2 ,m2);
        }
        if (strcmp(command, "num") == 0) {
            matrix = num_m(matrix, n, m);
        }
        if (strcmp(command, "det") == 0) {
            determinant_gauss(matrix, n1, m1);
        }
    }
}
