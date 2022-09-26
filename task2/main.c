#include "matrixlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char command[6];
    printf("Welcome to Matrix Calculator. To read the instruction, press 0.\n");
    scanf("%s", command);
    if(strcmp(command, "0") == 0){
        instruction();
    }
    double * matrix = input_m();
    output_m(matrix);
}