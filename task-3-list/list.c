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

void swap(List **a, List **b, List ** pa, List ** pb){
    List * tmp;

    tmp = (*pa) -> next;
    (*pa) -> next = (*pb) -> next;
    (*pb) -> next = tmp;

    printf("%s %s\n*\n", (*a) -> s, (*b) -> s);

    tmp = (*a);
    (*a) = (*b);
    (*b) = tmp;
    printf("%s %s\n", (*a) -> s, (*b) -> s);
}

void swapd(List **a, List **b){
    char * tmp;

    //printf("%s %s\n*\n", (*a) -> s, (*b) -> s);
    tmp = (*a) -> s;
    (*a) -> s = (*b) -> s;
    (*b) -> s = tmp;
    //printf("%s %s\n", (*a) -> s, (*b) -> s);
}


List *part(List *head, List *tail){
    List *curr = head;
    List *prev1, *prev2;
    List *q = head;

   prev1 = head;
   prev2 = curr;

    while(curr != NULL && curr != tail){
        if(strcmp(curr -> s, tail -> s) < 0){
            q = head;
            //printf("%s*\n", q -> s);
            if(strcmp(head -> s, curr -> s) != 0){
                //swapd(&head, &curr);
                swap(&head, &curr, &prev1, &prev2);
            }
            prev1 = head;
            head = head -> next;
            printf("%s %s\n*\n", prev1 -> s, head -> s);
        }
        prev2 = curr;
        curr = curr -> next;
        printf("%s %s\n**\n", prev2 -> s, curr -> s);
    }

    if(strcmp(head -> s, tail -> s) != 0){
       //swapd(&head, &tail);
       swap(&head, &tail, &prev1, &prev2);
    }
    return(q);
}

void sort(List **head, List **tail) {
    if ((*head) == (*tail)) {
        return;
    }

    List *q = part((*head), (*tail));
    if (q != NULL && q -> next != NULL) {
        sort(&(q -> next), tail);
    }
    if (q != NULL && (*head) != q) {
        sort(head, &q);
    }

}

int main() {
    char *s = get_S();
    List *head = NULL;
    List *tail = NULL;
    while(strcmp(s, END_WORD) != 0){
        push(&head, s);
        s = get_S();
    }
    tail = getTail(head);
  //  printf("%s\n", tail -> s);
    sort(&head, &tail);
    print(head);

    return 0;
}