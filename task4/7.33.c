#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADD_M 10
#define ADD_A 1

char * get_S(FILE *f, int *len, int maxlen) {
    int n, k = 0;
    char * s = malloc(maxlen * sizeof(char));

    if(!s) return NULL;
    while(fgets(s, maxlen, f)) {
        n = strlen(s);
        s[n - 1] = '\0';
        *len = n;
        return s;
    }
    return NULL;
}

int main(int argc, char * argv[]) {
    FILE * f; int max_len, len = 0, count = 0, n = ADD_M;
    f = fopen(argv[1], "r+");
    max_len = atoi(argv[2]);
    char *s1 = NULL, *s2 = NULL, *str = NULL;

    int *len_arr = calloc(n, sizeof(int)), *offset_arr = calloc(n, sizeof(int));
    char **str_arr = malloc(n * sizeof(char));
    printf("%d\n", max_len);

    if (f == NULL) {
        printf("Error opening file");
        exit(1);
    }

    while(1) {
        str = get_S(f, &len, max_len);
        if(str == NULL) break;
        str_arr[n - 1] = malloc(len + 1);
        strcpy(str_arr[n - 1], str);
        offset_arr[count] = ftell(f) - sizeof(int);
        len_arr[count] = len;
        count++;

        if(count >= n) {
            n += ADD_M;
            offset_arr = realloc(offset_arr, n * sizeof(int));
            len_arr = realloc(len_arr, n * sizeof(int));
        };
    }

    for(int i = 0; i < count - 1; i++) {
        for(int j = i; j < count; j++){
            if(len_arr[i] > len_arr[j]) {
                fseek(f, offset_arr[j], SEEK_SET);
                s1 = fgets(str, max_len, f);
                fseek(f, offset_arr[i], SEEK_SET);
                s2 = fgets(str, max_len, f);
                fseek(f, offset_arr[j], SEEK_SET);
                fwrite(&s2, sizeof(char), 1, f);
                fseek(f, offset_arr[i], SEEK_SET);
                fwrite(&s1, sizeof(char), 1, f);
            }
        }
    }

    fclose(f);
    return 0;
}