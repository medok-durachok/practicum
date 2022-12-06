#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define partA 0
#define partB 1
#define partC 2
#define modul 3
#define final_product 4

int semid;

void SigHndlr(int s) {
    semctl(semid, 0, IPC_RMID, (int) 0);
    semctl(semid, 1, IPC_RMID, (int) 0);
    semctl(semid, 2, IPC_RMID, (int) 0);
    semctl(semid, 3, IPC_RMID, (int) 0);
    semctl(semid, 4, IPC_RMID, (int) 0);
    exit(0);
}

int main(int argc, char **argv) {
    struct sembuf product[5];
    key_t key;
    key = ftok("/usr/sem", 's');
    semid = semget(key, 5, 0666 | IPC_CREAT);                               //создали массив семафоров
    if(semid == -1) {
        exit(1);
    }
    signal(SIGINT, SigHndlr);

    product[partA].sem_num = 0;     //А
    product[partA].sem_flg = 0;
    product[partB].sem_num = 1;     //В
    product[partB].sem_flg = 0;
    product[partC].sem_num = 2;     //С
    product[partC].sem_flg = 0;     
    product[modul].sem_num = 3;     //модуль
    product[modul].sem_flg = 0;
    product[final_product].sem_num = 4;     //продукт
    product[final_product].sem_flg = 0;
    product[final_product].sem_op = 1;

    semctl(semid, partA, SETVAL, (int) 0);                          //устанавливаем начальные значения 0 — как колво деталей
    semctl(semid, partB, SETVAL, (int) 0);
    semctl(semid, partC, SETVAL, (int) 0);
    semctl(semid, modul, SETVAL, (int) 0);
    semctl(semid, final_product, SETVAL, (int) 0);

    while(1) {
        product[partC].sem_op = -1;             
        product[modul].sem_op = -1;

        semop(semid, &product[partC], 1);
        semop(semid, &product[modul], 1);
        semop(semid, &product[final_product], 1);
        printf("DONE\n");
    }

    return 0;
}