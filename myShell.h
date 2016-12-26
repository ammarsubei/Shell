
int builtin_cd(char **args);
int builtin_help(char **args);
int builtin_exit(char **args);

char** check_redirection(char **oldArgs);
char** read_and_tokenize(int *argIndex);

void execute_command(char **oldArgs);
