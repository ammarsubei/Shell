#pragma once

#define MAXPATH 512
#define MAXLINE 4096
#define MAXARGS 21
#define MAXARGLENGTH 200

int check_builtin(char **args);
int builtin_cd(char **args);
int builtin_help(char **args);
void builtin_exit(char **args);

char **check_redirection(char **oldArgs);
void read_and_tokenize(char **args, int *argIndex);
void execute_command(char **oldArgs);