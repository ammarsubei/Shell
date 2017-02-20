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


// Function that prints out the prompt to the user
char *user_prompt(char *currentDirectory)
{
    if (getcwd(currentDirectory, sizeof(currentDirectory)))
        return currentDirectory;
    else
        perror("getcwd");
    return NULL;
}


// List of built-in shell commands
char *builtin_commands[] = {
    "cd",
    "help",
    "exit"
};


// Returns number of supported built-in commands
int num_builtins() {
    return sizeof(builtin_commands) / sizeof(char *);
}


// Function to support changing directories
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


// Function to support help command
int builtin_help(char **args)
{
    printf("\nAmmar Subei's Almond Shell\n");
    printf("Type program names and their arguments, then hit enter to execute.\n");
    printf("The following commands are built in:\n");

    for (int i = 0; i < num_builtins(); i++) 
        printf("  %s\n", builtin_commands[i]);


    printf("Use the man command for information on other programs.\n");

    return 1;
}


// Function to support exit command
void builtin_exit(char **args)
{
    printf("\nExiting Almond Shell... :(\n");
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
    
    else if (!strcmp(args[0], "exit"))
    {
        builtin_exit(args);
        return 1;
    }

    else
        return 0;
}


// Function that checks and applies any redirections from user command line
// This function is only called within the forked process
char **check_redirection(char **oldArgs)
{
    int redirects = 0;
    int index = 0;

    while (oldArgs[index] != NULL)
        index++;

    // For loop that checks for redirection
    // Uses freopen to change stdin/stdout file descriptors
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
    char **newArgs = (char**) malloc(index * sizeof(char*));
    for (int i = 0; i < index; i++)
        newArgs[i] = oldArgs[i];

    // Add final NULL element
    newArgs[index-1] = NULL;

    // Return our newly-created arguments array
    return newArgs;
}


// Function that forks the process, checks for any redirections,
// and runs the commands with any arguments accordingly
void execute_command(char **oldArgs)
{
    // If not using built-in command
    if (!check_builtin(oldArgs))
    {
        int status;
        pid_t pid = fork();

        // Error forking
        if (pid < 0)
            perror("Fork");

        // Child process
        else if (pid == 0)
        {
            // Check for any redirections and create a new arguments array
            char **newArgs = check_redirection(oldArgs);

            // Execute the command if it's not built-in
            if (execvp(*newArgs, newArgs) == -1)
                perror(*newArgs);

            exit(EXIT_FAILURE);
        }

        // Parent process
        else
        {
            // Wait for child process to die
            waitpid(pid, &status, 0);

            // Report if child went bonkers!
            if (!WIFEXITED(status))
                printf("Child terminated abnormally!\n");            
        }
    }
}


// Function that reads a line from user and processes it by tokenizing the line 
// using delimiters, and returns a new array of arguments composed of each string token
char **read_and_tokenize(int *argIndex, char *test)
{
    char delimiters[] = " \t\r\n\v\f";
    char *token = NULL;
    char *inputLine = (char*) malloc(MAXLINE * sizeof(char));
    char **args = (char**) malloc(MAXARGS * sizeof(char*));

    inputLine = readline(test);
    token = strtok(inputLine, delimiters);

    if (token == NULL)
        return NULL;

    // Tokenize line into arguments
    while (token != NULL)
    {
        args[*argIndex] = token;
        token = strtok(NULL, delimiters);
        (*argIndex)++;
    }
    // Add final NULL element
    args[*argIndex] = NULL;

    return args;
}


int main()
{
    // Welcome message
    printf("\nWelcome to Almond Shell!\n");
    printf("Please enter your desired commands below.\n");
    printf("Enter \"help\" for more information.\n");
    
	// Initialize variables
    int argIndex = 0;
    char **args = (char**) malloc(MAXARGS * sizeof(char*));
    char currentDirectory[MAXPATH];

	while (1)
	{
    	// Display prompt for user  
    	char *test = user_prompt(currentDirectory);


    	// Read input line from user and tokenize it
        // Every token will be inserted in the arguments array args
        args = read_and_tokenize(&argIndex, test);

        // If nothing was entered
        if (args == NULL)
            // Skip command execution
            continue;

        // Execute the command by passing in the args array
        execute_command(args);

    	// Reset argument array and its index
        for (int i = 0; i <= argIndex; i++)
            args[i] = NULL;

        argIndex = 0;
    	// Start again
	}
}