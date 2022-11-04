#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define ADD_M 20

char *keyboard_enter(void) {                        //ввод строк с клавиатуры
    int unsigned str_length, k = 0;
    char *s = malloc(ADD_M);
    if (!s) return NULL;

    fflush(stdout);
    printf("> ");
    while(fgets(s + k, ADD_M, stdin)) {
        str_length = strlen(s);
        if(s[str_length - 1] != '\n') {
            k = k + ADD_M - 1;
            s = realloc(s, k + ADD_M);

            if(!s) return NULL;
        } else {
            s[str_length - 1] = '\0';
            return s;
        }
    }
    return NULL;
}

char *file_enter(FILE *f) {                        //ввод из файла
   int unsigned str_length = 0, k = 0;

    char *s = malloc(ADD_M);
    if (!s) return NULL;

    while(fgets(s + k, ADD_M, f)) {
        str_length = strlen(s);
        if(s[str_length - 1] != '\n' && !feof(f)) {
            k = k + ADD_M - 1;
            s = realloc(s, k + ADD_M);

            if(!s) return NULL;
        } else {
            if(s[str_length - 1] == '\n') s[str_length - 1] = '\0';
            return s;
        }
    }
    return NULL;
}

void mem_all(char *word, int l_c, char **w_a, int *w_c) {       //выделение памяти на слова в массиве
    word[l_c] = '\0';
    w_a[*w_c] = malloc(l_c + 1);
    strcpy(w_a[*w_c], word);
    (*w_c)++;
}

char **parse(char **w_arr, char *s, int *counter, int *current_args) {                 //разбивка строк на слова
    int is_closing_quote_mark = 0, is_first_space = 0, is_prev_special = 0;
    int let_counter = 0, word_counter = *counter, word_mem = ((*counter) / 10 + 1) * ADD_M, let_mem = ADD_M;
    char *word = malloc(ADD_M);

    for(int i = 0; i < strlen(s); i++) {
        if(s[i] != ' ') {                                           //ниже: обработка спецсимволов
            if(s[i] == '&' || s[i] == '|' || s[i] == ';' || s[i] == '>' || s[i] == '<' || s[i] == '(' || s[i] == ')') {
                if(i != 0 && is_first_space != 1 && is_prev_special != 1) {                   //если спецсимвол первый в строке или стоит после пос-ти пробелов
                    if(word_counter == word_mem) {              //то нет предшествующего незаписанного слова
                        word_mem += ADD_M;
                        w_arr = realloc(w_arr, word_mem * sizeof(char*));
                    }
                    mem_all(word, let_counter, w_arr, &word_counter);
                    (*current_args)++;
                }

                let_counter = 0;
                word[let_counter] = s[i];
                if(((i + 1) != strlen(s)) && (s[i + 1] == '&' || s[i + 1] == '|' || s[i + 1] == '>')) {  //проверка для парных символов
                    let_counter++;
                    i++;
                    word[let_counter] = s[i];
                }
                let_counter++;
                mem_all(word, let_counter, w_arr, &word_counter);
                (*current_args)++;
                let_counter = 0;
                is_prev_special = 1;
                continue;
            }
            is_first_space = 0;
            is_prev_special = 0;
            if(let_counter == let_mem - 1) {
                let_mem += ADD_M;
                word = realloc(word, let_mem);
            }
            if(s[i] == '"' && is_closing_quote_mark == 0) {        //обработка кавычек
                is_closing_quote_mark = 1;
                continue;
            } 
            if(s[i] == '"' && is_closing_quote_mark == 1) {
                is_closing_quote_mark = 0;
                continue;
            }
            word[let_counter] = s[i];
            let_counter++;
        } else {
            if(is_closing_quote_mark == 1) {               //обработка пробелов внутри кавычек
                word[let_counter] = ' ';
                let_counter++;
                continue;
            }
            if(is_first_space != 0) continue;
            is_first_space = 1;

            if(let_counter != 0) {
                if(word_counter == word_mem) {
                    word_mem += ADD_M;
                    w_arr = realloc(w_arr, word_mem * sizeof(char*));
                }
                mem_all(word, let_counter, w_arr, &word_counter);
                (*current_args)++;
            }
            let_counter = 0;
        }
    }

    if(is_first_space == 0 && is_prev_special == 0) {                                 //запись последнего слова, если не пробел и не спецсимвол
        if(word_counter == word_mem) {
            word_mem += ADD_M;
            w_arr = realloc(w_arr, word_mem * sizeof(char*));
        }
        mem_all(word, let_counter, w_arr, &word_counter);
        (*current_args)++;
    }

    free(word);
    *counter = word_counter;
    return w_arr;
}

void output(char **arr, int size) {                         //вывод массива слов
    printf("-----------\n");
    if(size == 0) printf("No words entered.\n");
    for(int i = 0; i < size; i++) {
        printf("%s\n", arr[i]);
    }
}

int cd(char **argv, int argc) {                                             //выполнение cd
    char *s;
    if (argc == 1) {
        s = getenv("HOME");
        if (s == NULL) {
            return 1;
        } else chdir(s);
    } else if (argc > 2) {
        fprintf(stderr, "cd command accepts only 1 argument\n");
        return 1;
    } else if (chdir(argv[1]) != 0) {
        perror("cd error");
        return 1;
    }
    return 0;
}

int command_exec(char **argv, int argc) {                                   //выполнение команд
    pid_t pid;
    int status;
    if(strcmp(argv[0], "") == 0) {
        printf("Empty string\n");
        return 1;
    }

    if(strcmp(argv[0], "cd") == 0) {
        return cd(argv, argc);
    } else {
        pid = fork();
        if(pid == -1) {
            perror("error");
            return 1;
        } else  if (pid == 0) {                                    
            if(execv(argv[0], argv) == -1) perror("error");
            exit(0);
        } else {
            wait(&status);
        }
    }
    return 0;
}

void parse_exec(char **words_arr, char *s, int *count) {                            //создаем подмассив из текущей строки
    int cur_count = 0, c_count = *count;
    words_arr = parse(words_arr, s, &c_count, &cur_count);
    char *cur_arr[cur_count + 1];
    for(int i = 0; i < cur_count; i++) {
        cur_arr[i] = words_arr[c_count - cur_count + i];
    }
    cur_arr[cur_count] = NULL;

    command_exec(cur_arr, cur_count);
    printf("\n");
    *count = c_count;
}

int main(int argc, char *argv[]) {
    char c[256]; int count = 0, is_right_sym = 0;
    FILE *f;
    char *s = malloc(ADD_M);
    char **words_arr = malloc(ADD_M * sizeof(char*));

    printf("----------- SHELL INTERPRETER -----------\n");
    printf("To enter data from a file press 'f'. To enter from the keyboard press 'k': ");
    do {                                                     //проверка правильности введеных символов
        scanf("%s", c);
        if(strcasecmp(c, "k") == 0 || strcasecmp(c, "f") == 0) is_right_sym = 1;
        if(is_right_sym == 0) printf("Wrong input. Try again (f/k): ");
    } while(is_right_sym == 0);
    getchar();

    if(strcasecmp(c, "k") == 0) {
        printf("To stop entering press Ctrl+D.\n");
        while(s != NULL) {
            s = keyboard_enter();
            if(s != NULL) parse_exec(words_arr, s, &count);
        }
        printf("stopped\n");
    }
    if(strcasecmp(c, "f") == 0) {
        printf("Inputing from file..\n");
        f = fopen(argv[1], "r");
        while(s != NULL) {
            s = file_enter(f);
            if(s != NULL) parse_exec(words_arr, s, &count);
        }
        fclose(f);
    }

    printf("\n-----------------------------------------\n");

    free(s);
    free(words_arr);
    return 0;
}