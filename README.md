# Almond Shell
A simple implementation of a shell in C, written by Ammar Subei. Still a work-in-progress, and needs a decent amount of polishing.  
Now supports basic tab-completion for file system paths, and supports access to command history using the arrow keys.

Enter your desired command followed by its arguments (if any). Supports some built-in functions like "cd" and "exit". Makefile provided for compilation.

### Functionality
The shell's up-to-date functionality: 

1. Maximum of 20 arguments
2. Maximum of 2500 characters per line
3. Supports basic I/O redirection
  * Maximum of 2 different redirections per line
  * `command > filename` redirects the output of command to filename, overwriting existing contents of filename
  * `command >> filename` redirects the output of command to filename, appending to existing contents of filename
  * `command < filename` redirects command to read its input from filename
4. Supports changing current working directory
5. Supports basic tab-completion
6. Supports command history
7. **Does not** support background processes
8. **Does not** support command pipes

### TO-DOs
Here is a checklist of things I plan to work on for Almond Shell:

- [x] Update user prompt to show current directory
- [x] Support changing current directory
- [x] Provide user with `help` command
- [x] Support tab-completion/auto-complete
- [x] Support command history
- [ ] Support command pipes