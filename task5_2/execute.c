#include "shelllib.h"

int cd(char **argv, int argc) {                                             //выполнение cd
    char *s;
    if (argc == 1) {
        s = getenv("HOME");
        if (s == NULL) {
            return 1;
        } else chdir(s);
    } else if (argc > 2) {
        fprintf(stderr, "cd command accepts only 1 argument\n");
        return 1;
    } else if (chdir(argv[1]) != 0) {
        perror("cd error");
        return 1;
    }
    return 0;
}

int command_exec(char **argv, int argc) {                                   //выполнение команд
    pid_t pid;
    int status;
    if(strcmp(argv[0], "") == 0) {
        printf("Empty string\n");
        return 1;
    }

    if(strcmp(argv[0], "cd") == 0) {
        return cd(argv, argc);
    } else {
        pid = fork();
        if(pid == -1) {
            perror("error");
            return 1;
        } else  if (pid == 0) {                                    
            if(execv(argv[0], argv) == -1) perror("error");
            exit(0);
        } else {
            wait(&status);
            printf("\n");
        }
    }
    return 0;
}