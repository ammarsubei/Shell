# Almond Shell
Primitive shell in C, written by Ammar Subei.

### Functionality
The shell's up-to-date functionality: 

1. Maximum of 20 arguments
2. Maximum of 2500 characters per line
3. Supports basic I/O redirection
  * Maximum of 2 different redirections per line
  * `command > filename` redirects the output of command to filename, overwriting existing contents of filename
  * `command >> filename` redirects the output of command to filename, appending to existing contents of filename
  * `command < filename` redirects command to read its input from filename
4. Supports changing current directory
5. **Does not** support multiple child processes
6. **Does not** support background processes
7. **Does not** support command pipes

### TO-DOs
Here is a checklist of things I plan to work on for Almond Shell:

- [x] Support changing current directory
- [x] Provide user with `help` command
- [ ] Support multiple child processes
- [ ] Support command pipes
- [ ] Update user prompt to show current directory
