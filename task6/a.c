#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

struct sembuf detail_params;									//семафор детали А

int main(int argc, char **argv) {
	key_t key;			
    key = ftok("/usr/sem", 's');								//ну вот подключились к ресурсу да
    int semid = semget(key, 1, 0666 | IPC_CREAT);               //бахнули ид               
    if(semid == -1) {
        exit(1);
    }

	detail_params.sem_num = 0;					//инициализация
	detail_params.sem_flg = 0;

	while(1) {													//по сути бесконечно увеличиваем семафор-счетчик, да..? спим, потом инкриз
		detail_params.sem_op = 1;			//ну по идее.. здесь мы только увеличиваем -_-
		semop(semid, &detail_params, 1);

		sleep(2);												//и это все, и так аналогично для В и С ????? тяжело.
		printf("detail A\n");
	}
}