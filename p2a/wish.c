#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <ctype.h>

char error_message[30] = "An error has occurred\n";
int max_num_args = 30;
char **tmp_realloc_pp, **paths, *tmp_realloc_p;
int num_paths, max_path_len;
char** history;
int max_history_len = 100, cur_history_len=0;

void disp_err_msg()
{
	write(STDERR_FILENO, error_message, strlen(error_message));
	return;
}

void run_subprocess(const char* binary_path, char* const args[], int num_args)
{
	pid_t child_pid = fork();
	int rc;
	if (child_pid == 0)
    {	
        rc = execv(binary_path, args);
        if(rc==-1)
        	disp_err_msg();
        exit(0);
    }
    else if (child_pid > 0)
    {
        rc = wait(NULL);
        if(rc==-1)
        	disp_err_msg();
    }
    else
        disp_err_msg();
    return;
}

void free_all(char** args, int num_args)
{
	for(int i=0; i<num_args; i++)
	{
		free(args[i]);
	}
	free(args);
	return;
}

int get_args(char* buffer, char*** args)
{
	char *pch, *p1, *p2;
	char sym[2], c;
	int pch_len, num_args = 0;
	pch = strtok(buffer," \t");
	while (pch != NULL)
	{
		pch_len = strlen(pch);
		p1 = strchr(pch, '|');
		p2 = strchr(pch, '>');		
 
		if((pch_len>1)&&((p1!=NULL)||(p2!=NULL)))
		{
			for(int i=0; i<pch_len; i++)
			{
				if(((pch[i]=='>')||(pch[i]=='|')))
				{
					if(i>0)
					{
						c = pch[i];
						tmp_realloc_p = (char *)malloc(i+1);
						if (tmp_realloc_p != NULL)
				        	(*args)[num_args] = tmp_realloc_p;
						else
							return -1;
						pch[i] = '\0';
						strcpy((*args)[num_args], pch);
						pch[i] = c;
						pch = &pch[i];
						break;
					}
					else if(i==0)
					{
						sym[0] = pch[0];
						sym[1] = '\0';
						tmp_realloc_p = (char *)malloc(2);
						if (tmp_realloc_p != NULL)
				        	(*args)[num_args] = tmp_realloc_p;
						else
							return -1;
						strcpy((*args)[num_args], sym);
						pch = &pch[1];
						break;
					}
				}
			}
		}
		else
		{
			tmp_realloc_p = (char *)malloc(strlen(pch)+1);
			if (tmp_realloc_p != NULL)
		    	(*args)[num_args] = tmp_realloc_p;
			else
				return -1;

			strcpy((*args)[num_args], pch);
			pch = strtok(NULL, " ");
		}

		num_args++;
		if(num_args>(max_num_args-2))
		{
			max_num_args *= 2;
			tmp_realloc_pp = (char**)realloc((*args), max_num_args*sizeof(char*));
			if (tmp_realloc_pp != NULL)
            	(*args) = tmp_realloc_pp;
			else
				return -1;
		}
	}
	return num_args;
}

void run_command(char** args, int num_args)
{
	int flag;
	if(strcmp(args[0], "exit")==0)
	{
		if(num_args>1)
			disp_err_msg();
		else
			exit(0);
	}
	else if(strcmp(args[0], "cd")==0)
	{
		if(num_args==2)
		{
			flag = chdir(args[1]);
			if(flag==-1)
				disp_err_msg();
		}
		else
			disp_err_msg();
	}
	else if(strcmp(args[0], "history")==0)
	{
		if(num_args>2)
		{
			disp_err_msg();
			return;
		}
		int num_hist = cur_history_len+1;
		if(num_args==2)
		{
			int f = 1;
			char ch;
			for(int i=0; i<strlen(args[1]); i++)
			{
				ch = args[1][i]; 
				if(!((ch=='.')||(isdigit(ch)||(ch=='-'))))
				{
					f = 0;
					break;
				}
				if((ch=='.'))
				{
					if(f==10)
					{
						f = 0;
						break;
					}
					f = 10;
				}
			}
			if(f==0)
			{
				disp_err_msg();
				return;
			}
			if(atof(args[1])+1 <= cur_history_len)
				num_hist = (int)ceil(atof(args[1]))+1;
		}

		for(int i=(cur_history_len-num_hist+1); i<cur_history_len; i++)
		{
			printf("%s\n",history[i]);
		}
	}	
	else if(strcmp(args[0], "path")==0)
	{
		free_all(paths, num_paths);
		tmp_realloc_pp = (char**)malloc((num_args-1)*sizeof(char*));
		if (tmp_realloc_pp != NULL)
	    	paths = tmp_realloc_pp;
		else
		{
			disp_err_msg();
			return;
		}
			
		num_paths = num_args - 1;
		max_path_len = 0;
		for(int i=1; i<num_args; i++)
		{
			tmp_realloc_p = (char *)malloc(strlen(args[i])+2);
			if (tmp_realloc_p != NULL)
	        	paths[i-1] = tmp_realloc_p;
			else
			{
				disp_err_msg();
				return;
			}	
			strcpy(paths[i-1], args[i]);
			if(paths[i-1][strlen(paths[i-1])-1]!='/')
				strcat(paths[i-1],"/");
			if(strlen(paths[i-1])>max_path_len)
				max_path_len = strlen(paths[i-1]);
		}
	}		
	else
	{
		char* tmp_str = (char*)malloc((max_path_len+strlen(args[0]))*sizeof(char));
		if(tmp_str==NULL)
		{
			disp_err_msg();
			return;
		}
		char* cmd = (char*)malloc((strlen(args[0]))*sizeof(char));
		if(cmd==NULL)
		{
			free(tmp_str);
			disp_err_msg();
			return;
		}
		strcpy(cmd,args[0]);
		flag = 1;

		for(int i=0; i<num_paths; i++)
		{
			tmp_str[0] = '\0';
			strcpy(tmp_str, paths[i]);
			strcat(tmp_str, cmd);
			args[num_args] = NULL;
			if(access(tmp_str,X_OK) != -1)
			{
				run_subprocess(tmp_str, args, num_args);
				flag = 0;
				break;
			}
		}
		free(tmp_str);
		if(flag)
			disp_err_msg();
	}
}

int validate_args(char** args, int num_args, char* c)
{
	int pipe = 0;
	int redirect = 0;
	int ind = -2;
	for(int i=0; i<num_args; i++)
	{
		if(strcmp(args[i],"|")==0)
		{
			pipe++;
			ind = i;
			(*c) = '|';
		}
		else if(strcmp(args[i],">")==0)
		{
			redirect++;
			ind = i;
			(*c) = '>';
		}
	}
	if((pipe>1)||(redirect>1)||((pipe+redirect)>1)||(ind==num_args-1)||(ind==0))
		return -1;
	else 
		return ind;
}

void process_buffer(char* buffer)
{
	char** args = (char**)malloc(max_num_args*sizeof(char*));
	if(args==NULL)
	{
		disp_err_msg();
		return;
	}
	int num_args = get_args(buffer, &args);
	if(num_args==-1)
	{
		disp_err_msg();
		return;
	}
	if(num_args==0)
		return;

	char c = ' ';
	int val = validate_args(args, num_args, &c);
	if(val==-1)
	{
		disp_err_msg();
		return;
	}
	else if(val==-2)
		run_command(args, num_args);
	else
	{
		if(c=='>')
		{
			if(val==(num_args-2))
			{
				FILE* fp = fopen(args[val+1], "w+");
				if(fp==NULL)
				{
					disp_err_msg();
					return;
				}
				int dup_out = dup(fileno(stdout));
				int dup_err = dup(fileno(stderr));

				dup2(fileno(fp), fileno(stdout));
				dup2(fileno(stdout), fileno(stderr));
				fclose(fp);

				run_command(args,num_args-2);

				dup2(dup_out, fileno(stdout));
				dup2(dup_err, fileno(stderr));
			}
			else
			{
				disp_err_msg();
				return;
			}
		}
		else if(c=='|')
		{
			int fd[2];
			int rv = pipe(fd);
			if(rv==-1)
			{
				disp_err_msg();
				return;
			}

			int dup_out = dup(fileno(stdout));
			int dup_err = dup(fileno(stderr));
			int dup_in = dup(fileno(stdin));
			pid_t child_pid = fork();
			
			if (child_pid == 0)
			{	
				dup2(fd[1], fileno(stdout));
				dup2(fd[1], fileno(stderr));
				close(fd[0]);
				run_command(args,val);
				dup2(dup_in, fileno(stdin));
				exit(0);
			}
			else if (child_pid > 0)
			{
				dup2(fd[0], fileno(stdin));
				close(fd[1]);
				run_command(&args[val+1],num_args-val-1);
				int rc = wait(NULL);
			    if(rc==-1)
			    {
			    	disp_err_msg();
			    	return;
			    }
				dup2(dup_out, fileno(stdout));
				dup2(dup_in, fileno(stdin));
			}
			else
			{
			    disp_err_msg();
			    return;
			}
			dup2(dup_out, fileno(stdout));
			dup2(dup_in, fileno(stdin));
			dup2(dup_err, fileno(stderr));
		}

	}
	free_all(args, num_args);
	return;
}

void store_in_history(char* buffer)
{
	tmp_realloc_p = (char*)malloc(strlen(buffer)*sizeof(char*));
	if (tmp_realloc_p != NULL)
	{
    	history[cur_history_len] = tmp_realloc_p;	
	}
	else
	{
		disp_err_msg();
		return;
	}
	strcpy(history[cur_history_len], buffer);
	cur_history_len++;
	if(cur_history_len>max_history_len)
	{
		max_history_len *= 2;
		tmp_realloc_pp = (char**)realloc(history, max_history_len*sizeof(char*));
		if (tmp_realloc_pp != NULL)
	    	history = tmp_realloc_pp;
		else
		{
			disp_err_msg();
			return;
		}
	}
}

void run_loop(FILE* fp, int interactive)
{
	char* buffer = NULL;
	size_t n = 0;
	int k;

	num_paths = 1;
	tmp_realloc_pp = (char**)malloc(num_paths*sizeof(char*));
	if (tmp_realloc_pp != NULL)
	{
    	paths = tmp_realloc_pp;
	}
	else
	{
		disp_err_msg();
		return;
	}
	paths[0] = (char *)malloc(strlen("/bin/")+1);
	strcpy(paths[0],"/bin/");
	max_path_len = strlen(paths[0]);

	tmp_realloc_pp = (char**)malloc(max_history_len*sizeof(char*));
	if (tmp_realloc_pp != NULL)
	{
    	history = tmp_realloc_pp;
	}
	else
	{
		disp_err_msg();
		return;
	}
	
	while(1)
	{
		if(interactive==1)
		{
			printf("wish> ");
			fflush(NULL);
		}

		k = getline(&buffer, &n, fp);
		if(k==-1)
			break;
		
		if(strlen(buffer)>0)
		{
			if(buffer[k-1]=='\n')
				buffer[k-1] = '\0';
			if(strlen(buffer)>0)
				store_in_history(buffer);
			process_buffer(buffer);
		}
		buffer = NULL;
		n = 0;
		fflush(NULL);
	}
	return;
}

int main (int argc, char *argv[]) 
{
	if(argc==1)
		run_loop(stdin, 1);
	else if(argc==2)
	{
		FILE* fp = fopen(argv[1], "r");
         if(fp==NULL)
         {
            disp_err_msg();
            exit(1);
         }
         run_loop(fp, 0);
         fclose(fp);
	}
	else
	{
    	disp_err_msg();
    	exit(1);
	}
	return 0;
}