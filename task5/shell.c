#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADD_M 10
#define END_WORD "end"

char ** keyboard_enter(int *c){
    char ** words_arr;   
    int unsigned counter = 0;
    int unsigned str_length, k = 0;
    int unsigned n = ADD_M;     

    char * s = malloc(ADD_M);
    if (!s) return NULL;
    words_arr = malloc(n * sizeof(char *)); 

    do {
        while(fgets(s + k, ADD_M, stdin)){
            str_length = strlen(s);
            if(s[str_length - 1] != '\n'){
                k = k + ADD_M - 1;
                s = realloc(s, k + ADD_M);

                if(!s) return NULL;
            }
            else{
                s[str_length - 1] = '\0';
                break;
            }
        }

        if (strcmp(END_WORD, s) == 0) {     
            break;
        }

        if (counter >= n) {                             
            n += ADD_M;
            words_arr = realloc(words_arr, n * sizeof(char *));
        }

        words_arr[counter] = malloc(str_length + 1);
        strcpy(words_arr[counter], s);
        counter++;
    } while(1);

    *c = counter;
    return words_arr;
}

char ** file_enter(FILE *f, int *c) {
    char ** words_arr;   
    int unsigned s_len = 0, counter = 0;
    int unsigned k = 0;
    int unsigned n = ADD_M;     

    char * s = malloc(ADD_M);
    if (!s) return NULL;
    words_arr = malloc(n * sizeof(char *)); 

    do {
        while(fgets(s + k, ADD_M, f)) {
            s_len = strlen(s);
            if(s[s_len - 1] != '\n' && !feof(f)) {
                k = k + ADD_M - 1;
                s = realloc(s, k + ADD_M);
                if(!s) return NULL;
            } else {
                if(s[s_len - 1] == '\n') s[s_len - 1] = '\0';
                break;
            }
        }

        if (counter == n - 1) {                             
            n += ADD_M;
            words_arr = realloc(words_arr, n * sizeof(char *));
        }

        words_arr[counter] = malloc(strlen(s) + 1);
        strcpy(words_arr[counter], s);
        counter++;
    } while(!feof(f));

    *c = counter;
    return words_arr;
}

void output(char ** arr, int size){
    printf("\n");
    for(int i = 0; i < size; i++){
        printf("%s\n", arr[i]);
    }
}

int main() {
    char c; int count = 0;
    FILE *f;
    char ** words_arr;

    printf("----------- SHELL INTERPRETER -----------\n");
    printf("To enter data from a file, press 'f'. To enter from the keyboard, press 'k': ");
    scanf("%c", &c);

    if(c == 'k') {
        words_arr = keyboard_enter(&count);
    }
    if(c == 'f') {
        f = fopen("tmp.txt", "r");
        words_arr = file_enter(f, &count);
    }
    output(words_arr, count);
}