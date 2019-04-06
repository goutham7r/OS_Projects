CS-537 Project 1a
Goutham Ramakrishnan

Files submitted: my-cat.c, my-sed.c, my-uniq.c

Brief descriptions of implementation:

1. my-cat.c: Using a buffer of a 100 characters, retrieve a maximum of 100 characters using fgets at a time and use printf to display them. Stop when fgets returns NULL, i.e. there are no more characters to read. 

2. my-sed.c: Use getline function to read one line at a time. Use the strstr function to check if the 'find word' is present in the line. If not, print the line. If yes, then print upto the location returned by strstr. Then print the 'replace word'. Then print everything on the original line following the 'find word'.

3. my-uniq.c: Maintain two buffers. One which stores the previous line, one for the current line. Use the getline function to read one line at a time. Compare it with the contents of the 'previous' buffer using the strcmp function. If they are not the same, the print the line. Replace the previous buffer with the contents of the current buffer. 