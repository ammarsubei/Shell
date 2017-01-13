int check_builtin(char **args);
int builtin_cd(char **args);
int builtin_help(char **args);
void builtin_exit(char **args);

char** check_redirection(char **oldArgs);
char** read_and_tokenize(int *argIndex);

void execute_command(char **oldArgs);
void prompt_user();
