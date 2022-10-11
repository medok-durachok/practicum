#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADD_M 10
#define ADD_A 1

char * get_S(FILE *f, int *len) {                            //ввод динамической строки
    int n, k = 0;
    char * s = malloc(ADD_M);

    if(!s) return NULL;
    while(fgets(s + k, ADD_M, f)) {
        n = strlen(s);
        if(s[n - 1] != '\n') {
            k = k + ADD_M - 1;
            s = realloc(s, k + ADD_M);
            if(!s) return NULL;
        } else {
            s[n - 1] = '\0';
            *len = n;
            return s;
        }
    }
    return NULL;
}

int main(int argc, char * argv[]) {
    FILE * f1;
    f1 = fopen(argv[1], "r");
 
    if (f1 == NULL) {
        printf("Error opening file");
        exit(1);
    }

    int n = ADD_A;
    int isInArr = 0, maxim = -1, len = 0; 
    char **str_arr, *max_str = malloc(sizeof(char)); int *counter_arr;
    str_arr = malloc(n * sizeof(char*));
    counter_arr = calloc(n, sizeof(int));

    char *str = get_S(f1, &len);
    if(str == NULL) exit(1);
    str_arr[n - 1] = malloc(len + 1);
    strcpy(str_arr[n - 1], str);

    do {
        for(int i = 0; i < n; i++) {
            if(strcmp(str_arr[i], str) == 0) {
                counter_arr[i]++;
                isInArr = 1;
            }
            if(isInArr == 1) break;
        }
        if(isInArr == 0) {
            n++;
            str_arr = realloc(str_arr, n * sizeof(char*));
            counter_arr = realloc(counter_arr, n * sizeof(int));
            str_arr[n - 1] = malloc(len + 1);
            strcpy(str_arr[n - 1], str);
            counter_arr[n - 1] = 1;
        }
        isInArr = 0;
        str = get_S(f1, &len);
    } while (str != NULL);

    for(int i = 0; i < n; i++) {
        if(counter_arr[i] > maxim) {
            maxim = counter_arr[i];
            strcpy(max_str, str_arr[i]);
        }
    }
    printf("%s\n", max_str);

    fclose(f1);
    return 0;
}