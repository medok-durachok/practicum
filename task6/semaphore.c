#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct sembuf product;
int semid;

/*void detailA(void) {
    while(1) {
        sleep(2);
        product[0].sem_op = 1;
        if(semop(semid, product, 1) == -1) perror("detail production: ");
        printf("detail A\n");
    }
}
*/

int main(int argc, char **argv) {;
    key_t key;
    key = ftok("/usr/sem", 's');
    pid_t pid;
    semid = semget(key, 1, 0666 | IPC_CREAT);                               //создали массив семафоров
    if(semid == -1) {
        exit(1);
    }

    product.sem_num = 0;
    product.sem_flg = 0;
    /*product[1].sem_num = 1;
    product[1].sem_flg = SEM_UNDO;
    product[2].sem_num = 2;
    product[2].sem_flg = SEM_UNDO;
    product[3].sem_num = 3;
    product[3].sem_flg = SEM_UNDO;*/

    semctl(semid, 0, SETVAL, (int) 0);
    while(1) {
        product.sem_op = 2;
        semop(semid, &product, 1);

        product.sem_op = 0;
        semop(semid, &product, 1);
    }
                              
    /*if((pid = fork()) == -1) exit(1);
    if(pid == 0) {
        while(1) {
            sleep(2);
            printf("detail A\n");
            product[0].sem_op = 1;             
            if(semop(semid, product, 1) == -1) perror("detail production: ");
            exit(1);
        }
    } 
        product[0].sem_op = 0;
        product[1].sem_op = 0;
    }  */ 

    /*if((pid = fork()) == -1) exit(1);
    if(pid == 0) {
        sleep(3);
        printf("detail B\n");
        product[1].sem_op = 1;             
        if(semop(semid, product + 1, 1) == -1) perror("detail production: ");
        exit(1);
    } */     

    /*for(int i = 0; i < 3; i++) {
        if((pid = fork()) == -1) exit(1);
        if(pid == 0) {
            sleep(duration[i]);
            printf("detail %c\n", i + 'a');
            product[i].sem_op = 1;             
            semop(semid, product + i, 1);
            perror("detail production: ");
            exit(1);
        }          
    }*/

    /*product[0].sem_op = 0;
    product[1].sem_op = 0;
    semop(semid, product + 3, 1);
    printf("modul\n");
    //semop(semod, product, 2);*/

    semctl(semid, IPC_RMID, (int) 0);

    return 0;
}