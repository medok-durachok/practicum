#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADD_M 10

int main(int argc, char * argv[]) {
    FILE * f; int max_len, len = 0, count = 0, n = ADD_M;
    f = fopen(argv[1], "r+");
    max_len = atoi(argv[2]);
    char *str = malloc(max_len * sizeof(char));

    int *len_arr = calloc(n, sizeof(int)), *offset_arr = calloc(n, sizeof(int));

    if (f == NULL) {
        printf("Error opening file");
        exit(1);
    }

    offset_arr[0] = 0;
    while(fscanf(f, "%s", str) != -1) {
        len = strlen(str);
        str[len] = '\n';
        offset_arr[count + 1] = ftell(f) + 1;
        len_arr[count] = len;
        count++;

        if(count >= n) {
            n += ADD_M;
            offset_arr = realloc(offset_arr, n * sizeof(int));
            len_arr = realloc(len_arr, n * sizeof(int));
        };
    }
    free(str);

    FILE *tmp_f = fopen("tmp.txt", "w");
    if (tmp_f == NULL) {
        printf("Error opening file");
        exit(1);
    }

    for(int i = 0; i < count - 1; i++) {
        for(int j = 0; j < count - i - 1; j++){
            if(len_arr[j] > len_arr[j + 1]) {
                int tmp = len_arr[j];
                len_arr[j] = len_arr[j + 1];
                len_arr[j + 1] = tmp;

                tmp = offset_arr[j];
                offset_arr[j] = offset_arr[j + 1];
                offset_arr[j + 1] = tmp;
            }
        }
    }

    str = malloc(max_len* sizeof(char));
    for(int i = 0; i < count; i++) {
        fseek(f, offset_arr[i], SEEK_SET);
        fscanf(f, "%s", str);
        fwrite(str, sizeof(char*), 1, tmp_f);
        fputc('\n', tmp_f);
    }

    fclose(f);
    fclose(tmp_f);

    f = fopen(argv[1], "w");
    tmp_f = fopen("tmp.txt", "r");

    char c = ' ';
    while((c = fgetc(tmp_f)) != EOF) {
        fputc(c, f);
    }

    fclose(f);
    fclose(tmp_f);
    remove("tmp.txt");
    return 0;
}