/*
This is a simple implementation of a shell in C,
written by Ammar Subei.

Credit: Thanks to Stephen Brennan for help with
implementing the built-in functions. His tutorial
on writing a simple shell in C can be found here: 
https://brennan.io/2015/01/16/write-a-shell-in-c/
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "../include/myShell.h"


// List of built-in shell commands
char *builtin_commands[] = {
    "cd",
    "help",
    "exit"
};

// Returns number of supported built-in commands
int num_builtins() 
{
    return sizeof(builtin_commands) / sizeof(char *);
}

// Support changing directories
int builtin_cd(char **args)
{
    if (args[1] == NULL)
    {
        if (chdir(getenv("HOME"))) 
            perror("chdir");
    }

    else if (chdir(args[1])) 
        perror("chdir");

    return 1;
}

// Support help command
int builtin_help(char **args)
{
    printf("\nAmmar Subei's Almond Shell\n");
    printf("Type program names and their arguments, then hit enter to execute.\n");
    printf("The following commands are built in:\n");

    for (int i = 0; i < num_builtins(); i++) 
        printf("  %s\n", builtin_commands[i]);


    printf("Use the man command for information on other programs.\n");
    printf("Command history can be accessed using the arrow keys.\n");

    return 1;
}

// Support exit command
void builtin_exit(char **args)
{
    printf("\nExiting Almond Shell... :(\n");

    for (int i = 0; i < MAXARGS; i++)
        free(args[i]);
    free(args);

    exit(0);
}

// Check if built-in command used
int check_builtin(char **args)
{
    if (!strcmp(args[0], "help"))
        return builtin_help(args);

    else if (!strcmp(args[0], "cd"))
        return builtin_cd(args);

    else
        return 0;
}

// Checks for and applies any redirections from user input
char **check_redirection(char **oldArgs)
{
    int redirects = 0;
    int index = 0;

    while (oldArgs[index] != NULL)
        index++;

    // Uses freopen to change stdin/stdout file descriptors
    // NOTE: we don't need to reset file descriptors because
    // the child process will terminate anyway
    for (int i = 0; i < index; i++)
    {
        if (!strcmp(oldArgs[i], ">"))
        {
            freopen(oldArgs[i+1], "w", stdout);
            redirects++;
        }  

        else if (!strcmp(oldArgs[i], "<"))
        {
            freopen(oldArgs[i+1], "r", stdin);
            redirects++;
        }

        else if (!strcmp(oldArgs[i], ">>"))
        {
            freopen(oldArgs[i+1], "a", stdout);
            redirects++;
        }
    }
    // Subtract index by 2*redirects (since each redirect will take 2 elements from args)
    index -= 2*redirects;
    // Increment index to account for last NULL element
    index++;

    // Now create new args array that doesnt contain redirection symbols
    char **newArgs = (char **) malloc(index * sizeof(char *));
    for (int i = 0; i < index; i++)
        newArgs[i] = oldArgs[i];

    newArgs[--index] = NULL;

    return newArgs;
}

// Forks the process, checks for any redirections,
// and runs the commands with any arguments accordingly
void execute_command(char **oldArgs)
{
    if (!check_builtin(oldArgs))
    {
        int status;
        pid_t pid = fork();

        if (pid < 0)
            perror("Fork");

        // Child process
        else if (pid == 0)
        {
            char **newArgs = check_redirection(oldArgs);

            if (execvp(*newArgs, newArgs) == -1)
                perror(*newArgs);

            exit(EXIT_FAILURE);
        }

        // Parent process
        else
        {
            waitpid(pid, &status, 0);

            // Report if child went bonkers!
            if (!WIFEXITED(status))
                printf("Child terminated abnormally!\n");            
        }
    }
}

// Returns a new array of arguments composed of each string token
void read_and_tokenize(char **args, int *argIndex)
{
    char *delimiters = " \t\r\n\v\f";
    char *token = NULL;
    char *prompt = (char *) malloc(MAXPATH);
    char *inputLine = (char *) malloc(MAXLINE);

    if (!getcwd(prompt, MAXPATH))
        perror("Almond Shell");
    strcat(prompt, "$ ");

    inputLine = readline(prompt);

    if (!strcmp(inputLine, "exit"))
    {
        free(prompt);
        free(inputLine);
        builtin_exit(args);
    }

    if (inputLine)
        add_history(inputLine);

    // Tokenize the input line
    token = strtok(inputLine, delimiters);

    while (token != NULL)
    {
        strcpy(args[*argIndex], token);
        token = strtok(NULL, delimiters);
        (*argIndex)++;
    }

    // Add final NULL element
    args[*argIndex] = NULL;

    free(prompt);
    free(inputLine);
}

int main()
{
    // Welcome message
    printf("\nWelcome to Almond Shell!\n");
    printf("Please enter your desired commands below.\n");
    printf("Enter \"help\" for more information.\n");
    printf("NEW: Command history and tab-completion for file paths is now supported!\n\n");

    while (1)
    {
        int argIndex = 0;
        char **args = (char **) malloc(MAXARGS * sizeof(char *));

        for (int i = 0; i < MAXARGS; i++)
            args[i] = (char *) malloc(MAXARGLENGTH);

        // Read input line from user and tokenize it
        // Every token will be inserted in the arguments array 'args'
        read_and_tokenize(args, &argIndex);

        if (args == NULL)
            continue;

        execute_command(args);

        for (int i = 0; i < MAXARGS; i++)
            free(args[i]);

        free(args);
    }
}