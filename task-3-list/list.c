#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADD_M 10
#define END_WORD ""

typedef struct list {
	char * s;
	struct list * next;
} List;

char * get_S(void) {                            //ввод динамической строки
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

void push(List **head, char * s) {              //добавление узла в начало списка
    List *tmp = (List*) malloc(sizeof(List));
    tmp -> s = s;
    tmp -> next = (*head);
    (*head) = tmp;
}

void print(List *head) {                        // вывод списка
    List *p = head;
    while(p != NULL) {
        printf("%s\n", p -> s);
        p = p -> next;
    }
}

void delete(List **head) {                      //очистка памяти
    List *p = NULL;
    while((*head) -> next) {
        p = *head;
        (*head) = (*head) -> next;
        free(p);
    }
    free(*head);
}

void insert(List *ins, List **sorted) {         //вставка в отсортированную часть 
    if((*sorted) == NULL || (strcasecmp((*sorted) -> s, ins -> s) > 0)) {           //вставка первым или перед первым 
        ins -> next = (*sorted);
        (*sorted) = ins;
    } else {
        List *p = (*sorted);
        while((p -> next != NULL) && (strcasecmp(p -> next -> s, ins -> s) < 0)) {      //проход по элементам, меньшим нужного
            p = p -> next;      
        }
        ins -> next = p -> next;                                                    //вставка между элементами
        p -> next = ins;
    }
}

void sort(List **head) {                        // проход по неотсортированной части
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
    int flag = 0; List *head;
    do {
        printf("-----------\n%s\n", "Enter the words, each from a new line. To stop entering, leave the line empty.");
        head = NULL;
        char *s = get_S();
        if(strcmp(s, "") == 0 || strcmp(s, " ") == 0) {
            printf("\nNo words in the list");
        } else {
            while(strcmp(s, END_WORD) != 0) {
                if(strcmp(s, " ") != 0) push(&head, s);
                s = get_S();
            }
            sort(&head);
            printf("-----------\n%s\n", "Sorted list:");
            print(head);
        }
        printf("\n-----------\n%s\n", "Sort another one list? (y/n): ");

        while(1) {      
            char *c = get_S();                                        
            if(strcmp(c, "n") == 0) {
                if(head != NULL) delete(&head);
                flag = 1;
                printf("Exit..\n");
                break;
            }
            if(strcmp(c, "y") == 0) {
                if(head != NULL) delete(&head);
                break;
            } else {
                printf("Wrong input. Try again (y/n): ");
            }
        }

    } while(flag == 0);

    return 0;
}