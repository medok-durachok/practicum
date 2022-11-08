#ifndef SHELLLIB_H_INCLUDED
#define SHELLLIB_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define ADD_M 20

char *keyboard_enter(void);

char *file_enter(FILE *f);

void mem_all(char *word, int l_c, char **w_a, int *w_c);

char **parse(char **w_arr, char *s, int *counter, int *current_args);

void output(char **arr, int size);

void parse_exec(char **words_arr, char *s, int *count);

int cd(char **argv, int argc);

int command_exec(char **argv, int argc);

#endif