#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

void SigHndlr(int s) {
	remove("file");
	printf("Producing of detail is stopped\n");
	exit(0);
}

int main(int argc, char **argv) {
	struct sembuf detail_params;
	key_t key; int fd;
	fd = creat("file", 0666);
	if(fd < 0) exit(1);
    key = ftok("file", 's');
    int semid = semget(key, 4, 0666 | IPC_CREAT);                               
    if(semid == -1) {
    	perror("id");
        exit(1);
    }
    signal(SIGINT, SigHndlr);

	detail_params.sem_num = 2;
	detail_params.sem_flg = 0;

	while(1) {
		detail_params.sem_op = 1;
		sleep(4);		
		if(semop(semid, &detail_params, 1) == -1) exit(1);
		printf("detail C is produced\n");
	}
}