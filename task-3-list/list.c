#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADD_M 10
#define END_WORD ""

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


List * getTail(List *head) {
    if (head == NULL) {
        return NULL;
    }
    while (head->next) {
        head = head->next;
    }
    return head;
}

void push(List **head, char * s) {
    List *tmp = (List*) malloc(sizeof(List));
    tmp -> s = s;
    tmp -> next = (*head);
    (*head) = tmp;
}

void print(List *head) {
    List *p = head;
    while(p != NULL){
        printf("%s\n", p -> s);
        p = p -> next;
    }
}

void delete(List **head) {
    List *p = NULL;
    while((*head) -> next){
        p = *head;
        (*head) = (*head) -> next;
        free(p);
    }
    free(*head);
}

List * previous(List * head, List **current) {
    List * prev = head;
    while(prev -> next != *current) {
        prev = prev -> next;
    }
    return prev;
}

void insert(List *ins, List **sorted) {
    //printf("%s\n", ins -> s);
    if((*sorted) == NULL || (strcmp((*sorted) -> s, ins -> s) > 0)){
        ins -> next = (*sorted);
        (*sorted) = ins;
        //printf("%s %s\n*\n", (*sorted) -> s, ins -> s);
    } else {
        List *p = (*sorted);
        while((p -> next != NULL)&& (strcmp(p -> next -> s, ins -> s) < 0)) {
            p = p -> next;
        }
        ins -> next = p -> next;
        p -> next = ins;
        //printf("%s %s\n**\n", p -> s, ins -> s);
    }
}

void sort(List **head) {
    //printf("%s\n", (*head) -> s);
    List * curr = (*head);
    List *sorted = NULL;

    while (curr != NULL)
    {
        List *next_el = curr -> next;
        insert(curr, &sorted);
        curr = next_el;
    }
    *head = sorted;
}

int main() {
    char *s = get_S();
    List *head = NULL;
   // List *tail = NULL;
    while(strcmp(s, END_WORD) != 0){
        push(&head, s);
        s = get_S();
    }
    //tail = getTail(head);
  //  printf("%s\n", tail -> s);
   // sort(&head, &tail);
    print(head);
    printf("\n");
    sort(&head);
    print(head);

    return 0;
}