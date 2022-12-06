#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>								//семафор детали А

int main(int argc, char **argv) {
	struct sembuf detail_params;	
	key_t key;			
    key = ftok("2.txt", 's');								
    int semid = semget(key, 1, 0666 | IPC_CREAT);                            
    if(semid == -1) {
        exit(1);
    }

	detail_params.sem_num = 0;					//инициализация
	detail_params.sem_flg = 0;

	while(1) {													
		detail_params.sem_op = 1;	
		sleep(2);		
		if(semop(semid, &detail_params, 1) == -1) exit(1);												
		printf("detail A is produced\n");
	}
}
