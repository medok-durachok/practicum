#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADD_M 10
#define END_WORD ""

typedef struct list {
	char * s;
	struct list * next;
} List;

char * throwException(void) {
    char * c;
    c = malloc(ADD_M);
    printf("Wrong input. Try again (y/n): ");
    while(fgets(c, 256, stdin)) {
        return c;
    }
   // if(strcmp(c, "y") == 0 || strcmp(c, "n") == 0) {
        //break;
   // }
    return NULL;
}
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

void print(List *head) {
    List *p = head;
    while(p != NULL) {
        printf("%s\n", p -> s);
        p = p -> next;
    }
}

void delete(List **head) {
    List *p = NULL;
    while((*head) -> next) {
        p = *head;
        (*head) = (*head) -> next;
        free(p);
    }
    free(*head);
}

void insert(List *ins, List **sorted) {
    if((*sorted) == NULL || (strcmp((*sorted) -> s, ins -> s) > 0)) {
        ins -> next = (*sorted);
        (*sorted) = ins;
    } else {
        List *p = (*sorted);
        while((p -> next != NULL) && (strcmp(p -> next -> s, ins -> s) < 0)) {
            p = p -> next;
        }
        ins -> next = p -> next;
        p -> next = ins;
    }
}

void sort(List **head) {
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
    int flag = 0; char *c = malloc(ADD_M); List *head;
    do {
        printf("-----------\n%s\n", "Enter the words, each from a new line. To stop entering, leave the line empty.");
        head = NULL;
        char *s = get_S();
        if(strcmp(s, "") == 0) {
            printf("\nNo words in the list\n");
        } else {
            while(strcmp(s, END_WORD) != 0) {
                push(&head, s);
                s = get_S();
            }
            sort(&head);
            print(head);
        }
        printf("\n-----------\n%s\n", "Sort another one list? (y/n): ");
        while(fgets(c, 256, stdin)) {
          /* if((c[0] != 'n') && (c[0] != 'y')) {
                printf("Wrong input. Try again (y/n): ");
                //
            }*/
        exc:
            if(c[0] == 'n' && c[1] == '\n') {
                if(head != NULL) delete(&head);
                flag = 1;
                printf("Exit..\n");
                break;
            }
            if(c[0] == 'y' && c[1] == '\n') {
                delete(&head);
                break;
            } else {
                c = throwException();
                goto exc;
            }
        }

    } while(flag == 0);

    return 0;
}