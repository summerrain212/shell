#include "execute.h"
#include "parse_command.h"


int executeCommand(struct Command *command)
{
	int i,j;
	int argc = 0;
	// create pipe
	int default_stdout;
	
	int pid;
	int ret;
	int childNum = command->num_sub_commands;
	//int (*fds)[2] ; 
	int **fds;
	int stdout_copy = dup(STDOUT_FILENO);
	
	bool detect = false ;

	//TODO do not forget free
	fds = (int**)malloc((childNum-1)*sizeof(int));
	for (i = 0; i < childNum-1; i++)
	{
		fds[i] = (int*)malloc(2*sizeof(int));
	}

	/*
		spawn child processes 

	*/
	
	for (i = 0; i < childNum - 1; i++)
	{
		if (pipe(*(fds+i)) == -1)
		{
			perror("pipe");
			exit(1);
		}

	}

	//TODO 这里特别小心 现在是-1 为了是考虑最后一个有 <  > & command 同时这样保证了pipe n-1次
	for (i = 0; i < childNum ; i++)
	{
		//spawn the child 
		ret = fork();
		
		/* If this is the child process or error of creating the process 
		jump out of the for loop*/
		if (ret <= 0)
		{
			break;
		}
	}
	//Excute the childrprocess
	if (ret < 0)
	{
		perror("fork");
		return 1;
	}
	else if (ret == 0)
	{
		//For just one command
		if (childNum == 1)
		{
			if (command->stdin_redirect != 	NULL)
			{

				char target = '/';
				int i;
				char *fileName = command->stdin_redirect;
				for (i = 0; i < sizeof(fileName); i++)
				{
					if (fileName[i] == target)
					{
						detect = true;
						break;
					}
						
				}
				
				if (detect)
				{
					//reset value of detect
					detect = false;
					if(!is_path(fileName))
						exit(1);
				}	
				FILE *pFile = fopen (fileName, "a+");
				dup2(fileno(pFile),STDIN_FILENO);
				fclose (pFile);					
			}
			if (command->stdout_redirect != NULL)
			{
				char target = '/';
				int i;
				char *fileName = command->stdout_redirect;
				for (i = 0; i < sizeof(fileName); i++)
				{
					if (fileName[i] == target)
					{
						detect = true;
						break;
					}		
				}
				if (detect)
				{
					detect = false;

					if(!is_path(fileName))
						exit(1);
				}
				//printf ("hi\n");	
				FILE *pFile = fopen (fileName, "a+");
				dup2(fileno(pFile),STDOUT_FILENO);
				fclose (pFile);					
			    
			}
			if (command->background)
			{
				printf ("[%d]\n",getpid());
				//printf ("$ ");
				char curren_dir[256];
            	getcwd(curren_dir, sizeof(curren_dir));        
				printf ("%s$ ", curren_dir);
				fflush (stdout);

			}

			execvp(command->sub_commands[i].argv[0], command->sub_commands[i].argv);
			printf("%s: command not found\n", command->sub_commands[command->num_sub_commands-1].argv[0]);
			exit(1);
		}
		
		if (i == childNum - 1)
		{
			//TODO last subcommand
			close(*(fds[i-1]+1));
			for (j = 0; j < childNum - 2; j++)
			{
				close(*(fds[j]+0));
				close(*(fds[j]+1));
			}
			close(0);
			dup(*(fds[i-1]+0));

			if (command->stdin_redirect != 	NULL)
			{

				char target = '/';
				int i;
				char *fileName = command->stdin_redirect;
				for (i = 0; i < sizeof(fileName); i++)
				{
					if (fileName[i] == target)
					{
						detect = true;
						break;
					}
						
				}
				
				if (detect)
				{
					//reset value of detect
					detect = false;
					if(!is_path(fileName))
						exit(1);
				}	
				FILE *pFile = fopen (fileName, "a+");
				dup2(fileno(pFile),STDIN_FILENO);
				fclose (pFile);					
			}
			if (command->stdout_redirect != NULL)
			{
				char target = '/';
				int i;
				char *fileName = command->stdout_redirect;
				for (i = 0; i < sizeof(fileName); i++)
				{
					if (fileName[i] == target)
					{
						detect = true;
						break;
					}		
				}
				if (detect)
				{
					detect = false;

					if(!is_path(fileName))
						exit(1);
				}
				//printf ("hi\n");	
				FILE *pFile = fopen (fileName, "a+");
				dup2(fileno(pFile),STDOUT_FILENO);
				fclose (pFile);					
			    
			}
			if (command->background)
			{
				printf ("[%d]\n",getpid());
				//printf ("$ ");
				char curren_dir[256];
            	getcwd(curren_dir, sizeof(curren_dir));        
				printf ("%s$ ", curren_dir);
				fflush (stdout);

			}
			execvp(command->sub_commands[command->num_sub_commands-1].argv[0], command->sub_commands[command->num_sub_commands-1].argv);
			printf("%s: command not found\n", command->sub_commands[command->num_sub_commands-1].argv[0]);
			exit(1);

		}
		else 
		{
			if (i == 0)
	    	{
				for (j = 1; j < childNum - 1; j++)
				{
					close(*(fds[j]+0));
				}
				close(*(fds[i]+0));
	       		close(1);
				dup(*(fds[i]+1));
	    	}
	        else
	        {
	        	//The iout for command in middle
				for (j = 0; j < childNum - 1; j++)
				{
					if (j == i || j == i-1)
						break;
					close(*(fds[j]+0));
					close(*(fds[j]+1));
				}
				close(*(fds[i]+0));
	        	close(0);	
	        	//dup(fds[0]);
				dup(*(fds[i-1]+0));
				close(*(fds[i-1]+1));
				default_stdout = dup(1);	
	        	close(1);
	        	//dup(fds[1]);
				dup(*(fds[i]+1));
				
	    		
	        }
	        execvp(command->sub_commands[i].argv[0], command->sub_commands[i].argv);
	        dup2(default_stdout, 1);
            printf("%s: command not found\n", command->sub_commands[i].argv[0]);
			exit(1);

		}	
	}
	/*parent process */
	//if(ret > 0)
	else
    {
        /**parent need to close their pipe*/
		for (i = 0; i < childNum - 1; i++)
		{
			close(*(fds[i]+0));
			close(*(fds[i]+1));
		}

		if (!command->background) 
			//printf ("$ ");
		{
			char curren_dir[256];
            getcwd(curren_dir, sizeof(curren_dir));        
			printf ("%s$ ", curren_dir);
		}
			
		
        while((pid = wait(NULL)) > 0)
        {
			//这个只是最后一个subcommand
        	//sleep(1);
        	//printf("Process %d finished\n", pid);
        	//printf ("\n");
    	
        }
    }
}

bool is_path (char *path)
{
	/*
		existing 的file or
		existing的diectory 
	*/
	bool ret_value;
	char str[LINE_MAX];
	char *ptr;
	strcpy(str, path);
	//printf("before %s\n", str);
	ptr = strrchr(str, '/');
	str[strlen(str) - strlen(ptr)] = 0;
	//printf("after :%s\n", str);   

	struct stat file_st;
	if (stat(path,&file_st) == 0)
	{
		if( file_st.st_mode & S_IFDIR )
    	{
        	printf ("bash: %s " "Is a directory\n",path);
        	ret_value = false;
   		}
   		else if(file_st.st_mode & S_IFREG )
   		{
   			if (file_st.st_mode & S_IWOTH)
   			{
   				printf("Permission Denied\n");
   			}
   			ret_value = true;
   		}
   		else
   		{
   			ret_value =false;
   		}
	
	}
	else 
	{
		if (stat(str,&file_st) == 0)
		{
			if(file_st.st_mode & S_IFDIR)
    		{
        		//after remove is a directory
        		//printf ("After remove is a directory\n");
        		
        		ret_value = true;

    		}
    		else if(file_st.st_mode & S_IFREG )
   			{
   				//after remove is a f
   				ret_value = false;
   			}
   			else
   			{	
   				ret_value = false;
   			}
   		}
		else
		{
			if (errno == ENOENT)
    			printf("File not found\n");

			if(path[0] == '/')
			{
				printf ("Invalid path\n");
				ret_value = false;
			}
			else
			{
				printf ("bash :%s " "No such file or directory\n",path);
				ret_value = false;
			}
				
		
		}
	}
	return ret_value;
	

}
