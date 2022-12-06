#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

struct sembuf product[5];
int semid;

void SigHndlr(int s) {
    semctl(semid, IPC_RMID, (int) 0);
    kill(0, SIGTERM);
    exit(0);
}

int main(int argc, char **argv) {;
    key_t key;
    key = ftok("/usr/sem", 's');
    semid = semget(key, 1, 0666 | IPC_CREAT);                               //создали массив семафоров
    if(semid == -1) {
        exit(1);
    }
    signal(SIGINT, SigHndlr);

    product[0].sem_num = 0;     //А
    product[0].sem_flg = 0;
    product[1].sem_num = 1;     //В
    product[1].sem_flg = 0;
    product[2].sem_num = 2;     //С
    product[2].sem_flg = 0;     
    product[3].sem_num = 3;     //модуль
    product[3].sem_flg = 0;
    product[4].sem_num = 0;     //продукт
    product[4].sem_flg = 0;
    product[4].sem_op = 1;

    semctl(semid, 0, SETVAL, (int) 0);                          //устанавливаем начальные значения 0 — как колво деталей
    semctl(semid, 1, SETVAL, (int) 0);
    semctl(semid, 2, SETVAL, (int) 0);
    semctl(semid, 3, SETVAL, (int) 0);
    semctl(semid, 4, SETVAL, (int) 0);
                                              //по идее здесь начинаем бесконечное производство?? или все же 
    /*product[0].sem_op = 2;                              //бесконечность у нас только в процессах-производствах
    semop(semid, &product[0], 1);                       //в общем, здесь нам нужно проверить на не 0 деталь С и модуль.
    product[0].sem_op = 0;                              //если не 0, то уменьшаем их семафоры на 1; иначе блокируемся до получения не нуля
    semop(semid, &product[0], 1)*/
    while(1) {
        product[3].sem_op = -1;         //вот здесь ПО ИДЕЕ должно блокнуться по причине 0 + (-1) < 0
        product[4].sem_op = -1;

        semop(semid, &product[3], 2);
        printf("DONE\n");
        semop(semid, &product[4], 1);
    }

    return 0;
}