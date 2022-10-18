#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADD_M 10

int main(int argc, char * argv[]) {
    FILE * f; int max_len, len = 0, count = 0, n = ADD_M;
    f = fopen(argv[1], "r+");
    max_len = atoi(argv[2]);
    char *str = malloc((max_len + 1) * sizeof(char)), *s1 = malloc((max_len + 1) * sizeof(char)), *s2 = malloc((max_len + 1) * sizeof(char));

    int *len_arr = calloc(n, sizeof(int)), *offset_arr = calloc(n, sizeof(int));

    if (f == NULL) {
        printf("Error opening file");
        exit(1);
    }

    offset_arr[0] = 0;
    while(fscanf(f, "%s", str) != -1) {
        len = strlen(str);
        str[len] = '\0';
    //    printf("%d\n", len);
        offset_arr[count + 1] = ftell(f) + 1;
        printf("%d\n", offset_arr[count]);
        len_arr[count] = len;
        count++;

        if(count >= n) {
            n += ADD_M;
            offset_arr = realloc(offset_arr, n * sizeof(int));
            len_arr = realloc(len_arr, n * sizeof(int));
        };
    }
    free(str);

    for(int i = 0; i < count - 1; i++) {
        for(int j = i; j < count; j++){
            if(len_arr[i] > len_arr[j]) {
                int tmp = len_arr[i];
                len_arr[i] = len_arr[j];
                len_arr[j] = tmp;
                fseek(f, offset_arr[j], SEEK_SET);
                fscanf(f, "s", s2);
                fseek(f, offset_arr[i], SEEK_SET);
                fscanf(f, "s", s1);
              //  printf("%s %s\n", s1, s2);
                fseek(f, offset_arr[j], SEEK_SET);
                fwrite(s1, sizeof(char), 1, f);
                fseek(f, offset_arr[i], SEEK_SET);
                fwrite(s2, sizeof(char), 1, f);
            }
        }
    }

    fclose(f);
    return 0;
}