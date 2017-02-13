#pragma once

#define MAXPATH 1024
#define MAXLINE 2500
#define MAXARGS 21

int check_builtin(char **args);
int builtin_cd(char **args);
int builtin_help(char **args);
void builtin_exit(char **args);

char** check_redirection(char **oldArgs);
char** read_and_tokenize(int *argIndex, char *buffer);

void execute_command(char **oldArgs);
void prompt_user();
