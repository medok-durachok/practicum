#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ADD_M 10
#define END_WORD "end."

void output(char ** arr, int size){
    printf("\n");
    for(int i = 0; i < size; i++){
        printf("%s\n", arr[i]);
    }
}

void sort(char ** arr, int size){
    char * temp;
    for(int i = 0; i < size - 1; i++){
         for(int j = i + 1; j < size; j++){
            if(strcasecmp(arr[i], arr[j]) > 0){
                temp = arr[j];
                arr[j] = arr[i];
                arr[i] = temp;
            }
        }
    }
}

char ** enter(int unsigned * c_p){
    char ** words;   //массив указателей на char
    int unsigned counter = 0;
    int unsigned str_length, k = 0;
    int unsigned n = ADD_M;     //размер массива

    char * s = malloc(ADD_M);
    if (!s) return NULL;
    words = malloc(n * sizeof(char *)); //выделяем память под массив

    do {
        printf("%d: ", counter + 1);
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

        if (strcmp(END_WORD, s) == 0) {     //проверка строки на "концовку"
            break;
        }

        if (counter >= n) {                             //увеличиваем массив
            n += ADD_M;
            words = realloc(words, n * sizeof(char *));
        }

        words[counter] = malloc(str_length + 1);
        strcpy(words[counter], s);
        counter++;
    } while(1);

    *c_p = counter;
    return words;
}

int main()
{
    int unsigned counter = 0;
    printf("-----------\n%s\n", "Enter the words, each from a new line. To stop entering, print \"end\"");
    char ** arr = enter(&counter);

    if(counter == 0){
        printf("-----------\n%s", "No words in the list");
    }
    else{
        sort(arr, counter);

        printf("-----------\n%s", "Sorted list:");
        output(arr, counter);
    }

    for(int i = 0; i < counter; i++){
        free(arr[i]);
    }
    return 0;
}
