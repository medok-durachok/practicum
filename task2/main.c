#include "matrixlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char command[6];
    int n1, m1, n2, m2;
    double * matrix, * additional_matrix, * changed_matrix;
    printf("Welcome to Matrix Calculator. To read the instruction, press 0.\n");
    scanf("%s", command);
    if(strcmp(command, "0") == 0){
        instruction();
    }

    while(strcmp(command, "exit") != 0){
        printf("\nCommand: ");
        scanf("%s", command);
        if (strcmp(command, "add") == 0){
            scanf("\nEnter the number of rows: %d", n1);
            scanf("\nand the number of columns: %d", m1);
            matrix = input_m(n1, m1);
        }
        if (strcmp(command, "out") == 0){
            output_m(matrix);
        }
        if (strcmp(command, "+") == 0){
            printf("\nEnter the number of rows: ");
            scanf("%d", n2);
            printf("\nand the number of columns: ");
            scanf("%d", m2);
            additional_matrix = input_m(n2, m2);
            matrix = sum_m(matrix, additional_matrix, n1, m1, n2 ,m2);
        }
        if (strcmp(command, "*") == 0){
            printf("\nEnter the number of rows: ");
            scanf("%d", n2);
            printf("\nand the number of columns: ");
            scanf("%d", m2);
            additional_matrix = input_m(n2, m2);
            matrix = mul_m(matrix, additional_matrix, n1, m1, n2 ,m2);
        }
        if (strcmp(command, "num") == 0){
            matrix = num_m(matrix);
        }
        if (strcmp(command, "det") == 0){
            determinant_gauss(matrix, n1, m1);
        }
    }
}