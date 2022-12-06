#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {				//так ну, следуя блестящим умозаключениям, здесь мы чекаем не 0 в А и В, увеличиваем семафор модуля
	struct sembuf detail_params[2] = {			// и уменьшаем у деталей, ага. по сути гениально просто.
		{0, -1, 0},
		{1, -1, 0}
	};
	struct sembuf modul = {3, 1, 0};

	key_t key;
    key = ftok("/usr/sem", 's');
    int semid = semget(key, 1, 0666 | IPC_CREAT);                               
    if(semid == -1) {
        exit(1);
    }

	while(1) {
		semop(semid, &detail_params[0], 1);
		semop(semid, &detail_params[1], 1);
		printf("modul\n");
		semop(semid, &modul, 1);
	}
}