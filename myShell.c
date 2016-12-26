#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "myShell.h"

#define PROMPT "Almond> "
#define MAXLINE 2500
#define MAXARGS 21


// Function that prints out the prompt
void prompt_user()
{
    printf(PROMPT);
}

// Built-in shell commands
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &builtin_cd,
    &builtin_help,
    &builtin_exit
};

int num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

int builtin_cd(char **args)
{
    if (args[1] == NULL)
        fprintf(stderr, "Almond Shell: expected argument to \"cd\"\n");

    else 
        if (chdir(args[1]) != 0) 
            perror("Almond Shell");

    return 1;
}

int builtin_help(char **args)
{
    printf("Ammar Subei's Almond Shell\n");
    printf("Type program names and arguments, then hit enter to execute.\n");
    printf("The following commands are built in:\n");

    for (int i = 0; i < num_builtins(); i++) 
        printf("  %s\n", builtin_str[i]);


    printf("Use the man command for information on other programs.\n");
    return 1;
}

int builtin_exit(char **args)
{
    return 0;
}

// Function that checks if there are any redirections from user command line
// This function is only called within the forked process
char** check_redirection(char **oldArgs)
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
// and runs the commands accordingly
void execute_command(char **oldArgs)
{
    int status;
    pid_t pid, wpid;
    pid = fork();

    // Error forking
    if (pid < 0)
        perror("Almond Shell");

    // Child process
    else if (pid == 0)
    {
        // Check for any redirections and create a new arguments array
        char **newArgs = check_redirection(oldArgs);

        // Execute the command and print out any errors
        if (execvp(*newArgs, newArgs) == -1)
            perror(*newArgs);

        exit(EXIT_FAILURE);
    }

    // Parent process
    else
    {
        // Report the child process ID to user
        printf("PID: %d\n", pid);
        wpid = waitpid(pid, &status, 0);

        // Report to user the status of child process
        if (WIFEXITED(status))
            printf("Exit: %d\n", status);
        
        // Report if child went bonkers!
        else
            printf("Child terminated abnormally!\n");
    }
}

// Function that reads a line from user and processes it by tokenizing the line 
// using delimiters, and returns a new array of arguments composed of each string token
char** read_and_tokenize(int *argIndex)
{
    char delimiters[] = " \t\r\n\v\f";
    char *token = NULL;
    char *inputLine = (char*) malloc(MAXLINE * sizeof(char));
    char **args = (char**) malloc(MAXARGS * sizeof(char*));

    fgets(inputLine, MAXLINE, stdin);
    token = strtok(inputLine, delimiters);

    if (token == NULL)
        return NULL;

    if (!strcmp(token, "exit"))
    {
        printf("Exiting Almond Shell... :(\n");
        free(args);
        free(inputLine);
        exit(0);
    }

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
	// Initialize variables
    int argIndex = 0;
    char **args = (char**) malloc(MAXARGS * sizeof(char*));

	// Welcome message
    printf("Welcome to Almond Shell!\n");
    printf("Please enter your desired commands below.\n");
    printf("Enter \"help\" for more information.\n");

	while (1)
	{
    	// Prompt user to enter command  
    	prompt_user();

    	// Read input line from user and tokenize it
        // Every token will be inserted in the arguments array args
        args = read_and_tokenize(&argIndex);

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