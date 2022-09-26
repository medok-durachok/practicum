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

    while(strcmp(command, "exit" != 0)){
        scanf("\nCommand: %s", command);
        if (strcmp(command, "add")){
            matrix = input_m();
        }
        if (strcmp(command, "out")){
            output_m(matrix);
        }
        if (strcmp(command, "+")){
            matrix = sum_m(matrix, additional_matrix, n1, m1, n2 ,m2);
        }
        if (strcmp(command, "*")){
            matrix = mul_m(matrix, additional_matrix, n1, m1, n2 ,m2);
        }
        if (strcmp(command, "num")){
            matrix = num_m(matrix);
        }
        if (strcmp(command, "det")){
            determinant_gauss(matrix, n1, m1);
        }
    }
}