# Almond Shell
Primitive shell in C, written by Ammar Subei.

### Functionality
The shell's up-to-date functionality. 

1. Maximum of 20 arguments
2. Maximum of 2500 characters per line
3. Supports basic I/O redirection
  * Maximum of 2 different redirections per line
  * `command > filename` redirects the output of command to filename, overwriting existing contents of filename
  * `command >> filename` redirects the output of command to filename, appending to existing contents of filename
  * `command < filename` redirects command to read its input from filename
4. **Does not** support background processes
5. **Does not** support multiple child processes
6. **Does not** support changing directories

