#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADD_M 10

char *keyboard_enter(void) {
    int unsigned str_length, k = 0;

    char *s = malloc(ADD_M);
    if (!s) return NULL;

    fflush(stdout);
    printf("input> ");
    while(fgets(s + k, ADD_M, stdin)) {
        str_length = strlen(s);
        if(s[str_length - 1] != '\n') {
            k = k + ADD_M - 1;
            s = realloc(s, k + ADD_M);

            if(!s) return NULL;
        }
        else{
            s[str_length - 1] = '\0';
            return s;
        }
    }
    return NULL;
}

char * file_enter(FILE *f) {
    int unsigned s_len = 0;
    int unsigned k = 0;

    char * s = malloc(ADD_M);
    if (!s) return NULL;

    while(fgets(s + k, ADD_M, f)) {
        s_len = strlen(s);
        if(s[s_len - 1] != '\n' && !feof(f)) {
            k = k + ADD_M - 1;
            s = realloc(s, k + ADD_M);
            if(!s) return NULL;
        } else {
            if(s[s_len - 1] == '\n') s[s_len - 1] = '\0';
            return s;
        }
    }
    return NULL;
}

void mem_all(char *word, int l_c, char **w_a, int *w_c) {
    word[l_c] = '\0';
    w_a[*w_c] = malloc(l_c + 1);
    strcpy(w_a[*w_c], word);
    (*w_c)++;
}

char **parse(char **w_arr, char *s, int *counter) {
    int isCLosingQuoteMark = 0, isFirstSpace = 0;
    int let_counter = 0, word_counter = *counter, word_mem = ADD_M, let_mem = ADD_M;
    char *word = malloc(ADD_M);

    for(int i = 0; i < strlen(s); i++) {
        if(s[i] != ' ') {
            if(s[i] == '&' || s[i] == '|' || s[i] == ';' || s[i] == '>' || s[i] == '<' || s[i] == '(' || s[i] == ')') {
                if(i != 0 && isFirstSpace != 1) {
                    if(word_counter == word_mem - 1) {
                        word_mem += ADD_M;
                        w_arr = realloc(w_arr, word_mem * sizeof(char*));
                    }
                    mem_all(word, let_counter, w_arr, &word_counter);
                }

                let_counter = 0;
                word[let_counter] = s[i];
                if(((i + 1) != strlen(s)) && (s[i + 1] == '&' || s[i + 1] == '|' || s[i + 1] == '>')) {
                    let_counter++;
                    i++;
                    word[let_counter] = s[i];
                }
                let_counter++;
                mem_all(word, let_counter, w_arr, &word_counter);
                let_counter = 0;
                i++;
                if(i == strlen(s)) {
                    isFirstSpace = 1;
                    continue;
                }
            }
            isFirstSpace = 0;
            if(let_counter == let_mem - 1) {
                let_mem += ADD_M;
                word = realloc(word, let_mem);
            }
            if(s[i] == '"' && isCLosingQuoteMark == 0) {
                isCLosingQuoteMark = 1;
                continue;
            } 
            if(s[i] == '"' && isCLosingQuoteMark == 1) {
                isCLosingQuoteMark = 0;
                continue;
            }
            word[let_counter] = s[i];
            let_counter++;
        } else {
            if(isCLosingQuoteMark == 1) {
                word[let_counter] = ' ';
                let_counter++;
                continue;
            }
            if(isFirstSpace != 0) continue;
            isFirstSpace = 1;

            if(word_counter == word_mem - 1) {
                word_mem += ADD_M;
                w_arr = realloc(w_arr, word_mem * sizeof(char*));
            }
            mem_all(word, let_counter, w_arr, &word_counter);
            let_counter = 0;
        }
    }
    if(isFirstSpace == 0) {
        mem_all(word, let_counter, w_arr, &word_counter);
    }

    free(word);
    *counter = word_counter;
    return w_arr;
}

void output(char **arr, int size) {
    printf("-----------\n");
    if(size == 0) printf("No words entered.\n");
    for(int i = 0; i < size; i++) {
        printf("%s\n", arr[i]);
    }
}

void free_p(char **arr, int size) {
    for(int i = 0; i < size; i++) {
        free(arr[i]);
    }
}

int main() {
    char c[256]; int count = 0, isRightSym = 0;
    FILE *f;
    char *s = malloc(ADD_M);
    char **words_arr = malloc(ADD_M * sizeof(char*));

    printf("----------- SHELL INTERPRETER -----------\n");
    printf("To enter data from a file, press 'f'. To enter from the keyboard, press 'k': ");
    do{
        scanf("%s", c);
        if(strcasecmp(c, "k") == 0 || strcasecmp(c, "f") == 0) isRightSym = 1;
        if(isRightSym == 0) printf("Wrong input. Try again (f/k): ");
    } while(isRightSym == 0);
    getchar();

    if(strcasecmp(c, "k") == 0) {
        printf("To stop entering, press Ctrl+D.\n");
        while(s != NULL) {
            s = keyboard_enter();
            if(s != NULL) words_arr = parse(words_arr, s, &count);
        }
        printf("stopped\n");
    }
    if(strcasecmp(c, "f") == 0) {
        printf("Inputing from file..\n");
        f = fopen("tmp.txt", "r");
        while(s != NULL) {
            s = file_enter(f);
            if(s != NULL) words_arr = parse(words_arr, s, &count);
        }
        fclose(f);
    }

    output(words_arr, count);

    free(s);
    free_p(words_arr, count);
    free(words_arr);
    return 0;
}