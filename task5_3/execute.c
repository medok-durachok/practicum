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

void redirection(char **argv, int argc, short redir_in, short redir_out) {
    int index, f, both = 0;

    printf("in %d out %d\n", redir_in, redir_out);

    if(redir_out == 1) {
        index = find_sym(argv, argc, "<");                                      //здесь можно поймать ошибку при >/</>> в конце
        f = open(argv[index + 1], O_RDONLY);
        if(f == -1) {
            fprintf(stderr, "wrong reference to file.\n");
            exit(0);
        }
        dup2(f, 0);
        close(f);
    }
    if(redir_in != 0) {
        if(redir_in == 1) {
            index = find_sym(argv, argc, ">");                                      
            f = open(argv[index + 1], O_WRONLY | O_TRUNC | O_CREAT, 0664);
        } else {
            index = find_sym(argv, argc, ">>");                                      
            f = open(argv[index + 1], O_WRONLY | O_APPEND | O_CREAT, 0664);
        }
        if(f == -1) {
            fprintf(stderr, "wrong redirection input to file.\n");
            exit(0);
        }
        dup2(f, 1);
        close(f);
    }

    if(redir_out != 0 && redir_in != 0) {
        int f_help, i_i;
        index = find_sym(argv, argc, "<");                                      //ДУБЛИРОВАНИЕ !!
        f = open(argv[index + 1], O_RDONLY);
        if(f == -1) {
            fprintf(stderr, "wrong reference to file.\n");
            exit(0);
        }
        i_i = index;
        if(redir_in == 1) {
            index = find_sym(argv, argc, ">");                                      
            f_help = open(argv[index + 1], O_WRONLY | O_TRUNC | O_CREAT, 0664);
        } else {
            index = find_sym(argv, argc, ">>");                                      
            f_help = open(argv[index + 1], O_WRONLY | O_APPEND | O_CREAT, 0664);
        }
        if(f_help == -1) {
            fprintf(stderr, "wrong redirection output to file.\n");
            exit(0);
        }
        dup2(f, 0);
        dup2(f_help, 1);
        close(f);
        close(f_help);
        argv[i_i] = NULL;
    }
    argv[index] = NULL;
}

void pipeline(char **argv, int argc, int pipes) {
    int cmd_n = pipes + 1, status, index1 = 0, index2 = 0;
    int fd_help, fd[2];
    if(pipe(fd) == -1) exit(1);

    for(int i = 0; i <= cmd_n; i++) {                                   //первая команда конвейера
        if(i == 0) {     
            index2 = find_sym(argv, argc, "|");
            //доп массив
            index1 = index2;                                         
            if(fork() == 0) {
                dup2(fd[1], 1);
                close(fd[0]);
                close(fd[1]);
                //exec
            } 
            fd_help = fd[0];
            close(fd[1]);
        } else if(i == cmd_n) {                                            //последняя команда конвейера
            index2 = argc - 1;
            //
            if(fork() == 0) {
                dup2(fd_help, 0);
                close(fd_help);
                //exec
            } 
            close(fd_help);
        } else {
            index2 = find_sym(argv, argc, "|");
            //
            index1 = index2;
            if(fork() == 0) {
                dup2(fd_help, 0);
                dup2(fd[1], 1);
                close(fd[1]);
                close(fd_help);
                close(fd[0]);
                //exec
            }
            close(fd_help);
            close(fd[1]);
            fd_help = 0;
        }  
    }

    while(wait(&status) != -1);
}

int command_exec(char **argv, int argc, short is_redirection_in, short is_redirection_out, short is_pipe) {                                   //выполнение команд
    pid_t pid;
    int status;
    if(strcmp(argv[0], "") == 0) {
        printf("Empty string\n");
        return 1;
    }

    printf("%d\n" , is_pipe);

    if(strcmp(argv[0], "cd") == 0) {
        return cd(argv, argc);
    } else {
        pid = fork();
        if(pid == -1) {
            perror("error");
            return 1;
        } else  if (pid == 0) {  
            if(is_pipe != 0) pipeline(argv, argc, is_pipe);
            if(is_redirection_in != 0 || is_redirection_out != 0) redirection(argv, argc, is_redirection_in, is_redirection_out);                                  
            if(execvp(argv[0], argv) == -1) perror("error");
            exit(0);
        } else {
            wait(&status);
            printf("\n");
        }
    }

    return 0;   
}