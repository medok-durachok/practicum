#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {				
	struct sembuf detail_params[2] = {			
		{0, -1, 0},		//А
		{1, -1, 0}	 	//В
	};

	key_t key;
    key = ftok("2.txt", 's');
    int semid = semget(key, 2, 0666 | IPC_CREAT);                               
    if(semid == -1) {
        exit(1);
    }

	while(1) {
		if(semop(semid, &detail_params[0], 1) == -1) exit(1);
		if(semop(semid, &detail_params[1], 1) == -1) exit(1);
		printf("modul is produced\n");
		semop(semid, &detail_params[2], 1);
	}
}