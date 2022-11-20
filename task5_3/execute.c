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
    int index, f;

    if(redir_out == 1 && redir_in == 0) {
        index = find_sym(argv, argc, "<");                                      //здесь можно поймать ошибку при >/</>> в конце
        if(index == 0 || index == argc - 1) {
            printf("Wrong syntax.\n");
            exit(0);
        }
        f = open(argv[index + 1], O_RDONLY);
        if(f == -1) {
            fprintf(stderr, "wrong reference to file.\n");
            exit(0);
        }
        dup2(f, 0);
        close(f);
    }
    if(redir_in != 0 && redir_out == 0) {
        if(redir_in == 1) {
            index = find_sym(argv, argc, ">");                                    
            f = open(argv[index + 1], O_WRONLY | O_TRUNC | O_CREAT, 0664);
        } else {
            index = find_sym(argv, argc, ">>");                                      
            f = open(argv[index + 1], O_WRONLY | O_APPEND | O_CREAT, 0664);
        }
        if(index == 0 || index == argc - 1) {
            printf("Wrong syntax.\n");
            exit(0);
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

void cmd_exec(char **sub_arr, int argc, int redir_err, pid_t pid, short is_back) {
    short in = 0, out = 0;

    if(is_back) signal(SIGINT, SIG_IGN);
    else signal(SIGINT, SIG_DFL);

    if(find_sym(sub_arr, argc, "<") != -1) out = 1;     
    if(find_sym(sub_arr, argc, ">") != -1) in = 1;
    if(find_sym(sub_arr, argc, ">>") != -1) in = 2;

    if((in != 0 || out != 0) && redir_err == 1) {
        printf("Cannot be executed.\n");
        exit(0);
    } else {
        if(in != 0 || out != 0) redirection(sub_arr, argc, in, out);
        if(is_back) {
            setpgid(0, pid);                       
            int f = open("/dev/null", O_RDONLY);   
            dup2(f, 0);
        }
        if(execvp(sub_arr[0], sub_arr) == -1) perror("error");
    }
}

int pipeline(char **argv, int argc, int pipes) {
    int cmd_n = pipes + 1, status, index1 = -1, index2 = 0;
    int fd_help, fd[2]; short is_forked = 0, is_back = 0;
    pid_t pid, pgid;
    char **sub_arr;

    if(strcmp(argv[0], "cd") == 0) return cd(argv, argc);

    if(strcmp(argv[0], "") == 0) {
        return 0;
    }
    if(strcmp(argv[argc - 1], "&") == 0) {                     //фоновый процесс?
        is_back = 1; 
        argv[argc - 1] = NULL;
    }
    if(is_back == 1 && argc == 1) {
        printf("No background commands.\n");
        return 0;
    }

    if (is_back) {
        if (getpgrp() != getpid()) setpgid(0, 0);             
        else if ((pid = fork()) == 0) pipeline(argv, argc, pipes);
        else return 0;
    }
    pgid = getpgrp();

    if(pipes == 0) {
        if(fork() == 0) {
            cmd_exec(argv, argc, 0, pgid, is_back);
            exit(0);
        }
    } else {
        for(int i = 1; i <= cmd_n; i++) {                                  
            if(pipe(fd) == -1) return 1;
            if(i != cmd_n) {
                index2 = find_sym(argv, argc, "|");
                strcpy(argv[index2], "0");
            } else index2 = argc;
            sub_arr = sub_create(argv, index1, index2);

            if(i == 1) {                                    
                if(fork() == 0) {
                    dup2(fd[1], 1);
                    close(fd[0]);
                    close(fd[1]);
                    cmd_exec(sub_arr, index2 - index1 - 1, 0, pgid, is_back);
                }
                index1 = index2;
                fd_help = fd[0];
                close(fd[1]);
            } else if(i == cmd_n) {                                            //последняя команда конвейера
                if(fork() == 0) {
                    dup2(fd_help, 0);
                    close(fd_help);
                    cmd_exec(sub_arr, index2 - index1 - 1, 0, pgid, is_back);
                } 
                close(fd_help);
            } else {
                if(fork() == 0) {
                    dup2(fd_help, 0);
                    dup2(fd[1], 1);
                    close(fd[1]);
                    close(fd_help);
                    close(fd[0]);
                    cmd_exec(sub_arr, index2 - index1 - 1, 1, pgid, is_back);
                } 
                index1 = index2;
                close(fd_help);
                close(fd[1]);
                fd_help = 0;
            }  
        }
    }

    while(wait(NULL) != -1);
    if(pipes != 0) free(sub_arr);
    return 0;
}

/*int command_exec(char **argv, int argc, short is_redirection_in, short is_redirection_out, short is_pipe) {                                   //выполнение команд
    pid_t pid, pgid;
    int status; short is_back = 0;
    if(strcmp(argv[0], "") == 0) {
        return 0;
    }

    if(strcmp(argv[argc - 1], "&") == 0) {                     //фоновый процесс?
        is_back = 1; 
        argv[argc - 1] = NULL;
    }

    if(is_back == 1 && argc == 1) {
        printf("No background commands.\n");
        exit(0);
    }

    if(is_back) {
        signal(SIGINT, SIG_IGN);
    } else {
        signal(SIGINT, SIG_DFL);
    }

    if(strcmp(argv[0], "cd") == 0) {
        return cd(argv, argc);
    } else {
        pid = fork()

        if(pid == -1) {
            perror("error");
            return 1;
        } else  if (pid == 0) {  
            if(is_pipe != 0) {
                pipeline(argv, argc, is_pipe, is_back);
            } else {
                if(is_redirection_in != 0 || is_redirection_out != 0) redirection(argv, argc, is_redirection_in, is_redirection_out);                                  
                if(is_back) {
                    setpgid (getpid (), getpid ());                       
                    int f = open("/dev/null", O_RDONLY);   
                    dup2(f, 0);
                }
                if(execvp(argv[0], argv) == -1) perror("error");
            }
            exit(0);
        } else {
            wait(&status);
            printf("\n");
        }
    }

    return 0;   
}*/