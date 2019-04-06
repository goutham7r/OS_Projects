CS-537 Project 2a
Goutham Ramakrishnan

All code is present in wish.c. 

Brief descriptions of functions:
1. main(): parses input arguments. Enters Interactive or Batch mode as needed by calling run_loop().
2. run_loop(): Sets initial path. Gets command input line by line. Calls the process_buffer() function to execute command. Stores command in history array. 
3. process_buffer(): Calls the get_args() and validate_args() functions to split input buffer into arguments and to check that it is a valid command. Calls run_command() appropriately, after accounting for necessary redirection or piping. (Redirection and piping done using the dup(), dup2(), pipe() and fork() system calls). 
4. get_args(): Splits the input buffer into an array of arguments. 
5. validate_args(): Checks whether the argument array is valid (eg. only a single pipe or redirection is allowed). 
6. run_command(): 
7. store_in_history(): stores the input buffer in history array. Expands array if necessary. 
8. disp_err_msg(): Prints error message to stderr. 
9. run_command(): takes array of arguments as input. Checks if first argument is built in command. If so, does the appropriate task. If not, searches the path folders for the appropriate executable. If found, calls run_subprocess(). Otherwise, returns error message. 
10. run_subprocess(): Forks a child process which runs the binary specified in the path. Waits until it executes and then returns. 
11. free_all(): generic function to free memory allocated on the heap. 