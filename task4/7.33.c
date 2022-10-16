#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADD_M 10

char * get_S(FILE *f, int max_len) {
    int n, k = 0;
    char * s = malloc(max_len);

    if(!s) return NULL;
    while(fgets(s, max_len, f)) {
        n = strlen(s);
        if(s[n - 1] != '\n' && !feof(f)) {
            k = k + max_len - 1;
            if(!s) return NULL;
        } else {
            return s;
        }
    }
    return NULL;
}


int get_L(FILE *f, int max_len) {
    int n, k = 0;
    char * s = malloc(max_len);

    if(!s) return NULL;
    while(fgets(s, max_len, f)) {
        n = strlen(s);
        if(s[n - 1] != '\n' && !feof(f)) {
            k = k + max_len - 1;
            if(!s) return -1;
        } else {
            if(s[n - 1] == '\n') {
                s[n - 1] = '\0';
                return n - 1;
            } else {
                return n;
            }
        }
    }
    return -1;
}

int main(int argc, char * argv[]) {
    FILE * f; int max_len, len = 0, count = 0, n = ADD_M;
    f = fopen(argv[1], "rb+");
    max_len = atoi(argv[2]);
    char *str = NULL, *s1 = NULL, *s2 = NULL;

    int *len_arr = calloc(n, sizeof(int)), *offset_arr = calloc(n, sizeof(int));

    if (f == NULL) {
        printf("Error opening file");
        exit(1);
    }

    offset_arr[0] = 0;
    while((len = get_L(f, max_len)) != -1) {
        //printf("%d\n", len);
        offset_arr[count + 1] = ftell(f);
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
                s2 = get_S(f, max_len);
                fseek(f, offset_arr[i], SEEK_SET);
                s1 = get_S(f, max_len);
                printf("%s %s\n", s1, s2);
               /* fseek(f, offset_arr[j], SEEK_SET);
                fwrite(s1, sizeof(char), 1, f);
                fseek(f, offset_arr[i], SEEK_SET);
                fwrite(s2, sizeof(char), 1, f);*/
            }
        }
    }

    fclose(f);
    return 0;
}