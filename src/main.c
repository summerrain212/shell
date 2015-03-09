#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>	/* needed to define getpid() */
#include <stdbool.h>
#include "parse_command.h"
#include "execute.h"

int cd_cmd(struct Command *command)
{
    if(command->num_sub_commands != 1)
    {
        return -1;
    }
    
    if(strcmp(command->sub_commands[0].argv[0], "cd") == 0)
    {
        chdir(command->sub_commands[0].argv[1]);
        return 0;       
    }
    
    return -1;
}

int main( )
{
	char s[LINE_MAX];
    char curren_dir[256];  
    int argc;
    
    /*
    	Alternatively , we can also use struct pointer like below
    	struct Command *input_command = (struct Command *)malloc(sizeof(struct Command));	
    	readCommand(s, input_command);
    	printCommand(input_command);
	*/
    struct Command input_command;
    memset(&input_command, 0, sizeof(struct Command));
    //Initalize the Command
    
    
    //Implement the prompt $ followed by space
    getcwd(curren_dir, sizeof(curren_dir));
    printf("%s$ ", curren_dir);

    while (fgets(s, sizeof(s), stdin) != NULL)
    {
        if(strlen(s) == 1)
        {
            getcwd(curren_dir, sizeof(curren_dir));
            printf("%s$ ", curren_dir);            
            continue;
        }
        
    	s[strlen(s) - 1] = 0;
    	input_command.background = 0;
    	input_command.stdin_redirect = NULL;
    	input_command.stdout_redirect = NULL;

    	readCommand(s, &input_command);
    	//printf("background %d\n",input_command.background);
        
        if(cd_cmd(&input_command) == 0)
        {
            getcwd(curren_dir, sizeof(curren_dir));
            printf("%s$ ", curren_dir);            
            continue;
        }
		
		executeCommand (&input_command);

    	//printCommand(&input_command);
    }
    
    
    return 0;
}


