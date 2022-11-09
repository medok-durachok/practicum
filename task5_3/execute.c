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

int command_exec(char **argv, int argc, int is_redirection, int is_pipe) {                                   //выполнение команд
    pid_t pid;
    int status;
    if(strcmp(argv[0], "") == 0) {
        printf("Empty string\n");
        return 1;
    }

    if(is_redirection != -1) {
        int index, f;
        if(is_redirection == 0) {
            index = find_sym(argv, argc, "<");                                      //здесь можно поймать ошибку при >/</>> в конце
            f = open(argv[index + 1], O_RDONLY);
            if(f == -1) {
                fprintf(stderr, "wrong reference to file.\n");
                goto cont;
            }
            dup2(f, 0);
            close(f);
        } else {
            if(is_redirection == 1) {
                index = find_sym(argv, argc, ">");                                      
                f = open(argv[index + 1], O_WRONLY | O_TRUNC | O_CREAT, 0664);
            } else {
                index = find_sym(argv, argc, ">>");                                      
                f = open(argv[index + 1], O_WRONLY | O_APPEND | O_CREAT, 0664);
            }
            if(f == -1) {
                fprintf(stderr, "wrong redirection output to file.\n");
                goto cont;
            }
            dup2(f, 1);
            close(f);
        }
        argv[index] = NULL;
    }

cont:
    if(strcmp(argv[0], "cd") == 0) {
        return cd(argv, argc);
    } else {
        pid = fork();
        if(pid == -1) {
            perror("error");
            return 1;
        } else  if (pid == 0) {                                    
            if(execvp(argv[0], argv) == -1) perror("error");
            exit(0);
        } else {
            wait(&status);
            printf("\n");
        }
    }

    return 0;   
}