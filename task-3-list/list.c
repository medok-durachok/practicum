#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADD_M 10
#define END_WORD "end"

typedef struct list {
	char * s;
	struct list * next;
} List;

char * get_S(void) {
    int n, k = 0;
    char * s = malloc(ADD_M);

    if(!s) return NULL;

    while(fgets(s + k, ADD_M, stdin)) {
        n = strlen(s);
        if(s[n - 1] != '\n') {
            k = k + ADD_M - 1;
            s = realloc(s, k + ADD_M);
            if(!s) return NULL;
        } else {
            s[n - 1] = '\0';
            return s;
        }
    }
    return NULL;
}

void push(List **head, char * s) {
    List *tmp = (List*) malloc(sizeof(List));
    tmp -> s = s;
    tmp -> next = (*head);
    (*head) = tmp;
}

void print(List *head){
    List *p = head;
    while(p != NULL){
        printf("%s\n", p -> s);
        p = p -> next;
    }
}

int main() {
    char *s = get_S();
    List *head = NULL;
    while(strcmp(s, END_WORD) != 0){
        push(&head, s);
        s = get_S();
    }
    print(head);

    return 0;
}