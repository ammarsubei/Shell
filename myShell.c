#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROMPT "Almond> "
#define MAXLINE 2500
#define MAXARGS 21


// Function that prints out the prompt
void promptUser()
{
	printf(PROMPT);
}

// Function that checks if there are any redirections from user command line
// This function is only called within the forked process
char** checkRedirection(char **oldArgs)
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
void executeCommand(char **oldArgs)
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
        char **newArgs = checkRedirection(oldArgs);

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
char** readAndTokenize(int *argIndex)
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

	while (1)
	{
    	// Prompt user to enter command  
    	promptUser();

    	// Read input line from user and tokenize it
        // Every token will be inserted in the arguments array args
        args = readAndTokenize(&argIndex);

        // If nothing was entered
        if (args == NULL)
            // Skip command execution
            continue;

        // Execute the command by passing in the args array
        executeCommand(args);

    	// Reset argument array and its index
        for (int i = 0; i <= argIndex; i++)
            args[i] = NULL;

        argIndex = 0;
    	// Start again
	}
}