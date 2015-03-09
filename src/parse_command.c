#include "parse_command.h"
/*
	read the command line and divid into each sub commands
*/
void readCommand(char *line, struct Command *command)
{
	int subCmdArgc = 0;
	char *delimiter = "|";
	int i = 0;

	/*Initalize the pointer to avoid the wild pointer*/
	char *seg_str =  NULL;

	if (line == NULL)
	{
		printf("Error Input !\n");
	}

	seg_str = strtok (line, delimiter);

	for (; seg_str != NULL && subCmdArgc < MAX_SUB_COMMANDS; subCmdArgc++)
	{
		
		command->sub_commands[subCmdArgc].line = strdup(seg_str);

	 	seg_str = strtok(NULL, delimiter);

	}

	/*for future print argc starts from 0*/
	command->num_sub_commands = subCmdArgc;

	/*parse first argc-1 subcommand's argument*/
	for(i; i < subCmdArgc - 1 ; i++)
	{
	    readArgs(command->sub_commands[i].line, command->sub_commands[i].argv, MAX_ARGS) ;
	}

	readRedirectAndBackground(command);

	env_var_process(command->sub_commands);

		
}


int stringCmp (char *str)
{
	if(strcmp (str, "&")==0)
	{
		return 0;
	}
	else if (strcmp (str, "<")==0)
	{
		return 1;
	}
	else if (strcmp (str, ">")==0)
	{
		return 2;
	}
	else
		return 3;
}


/*
	parse the subcommand which has redirectin, redirectout, background (possible)
*/
void readRedirectAndBackground(struct Command *command)
{
	int i = 0;
	int argc = readArgs(command->sub_commands[command->num_sub_commands-1].line, command->sub_commands[command->num_sub_commands-1].argv, MAX_ARGS);
	//scan the arguments from last subcommand in reverse order
	//last argument in subcommand is NULL
	if(argc == MAX_ARGS)
		argc = argc - 2;
	else
		argc = argc - 1 ;
	for (argc; argc >= 0; argc--)
	{
		int flag = stringCmp(command->sub_commands[command->num_sub_commands-1].argv[argc]);

		//if (command->sub_commands[command->num_sub_commands-1].argv[argc])
		switch (flag)
		{
			case 0 :
				command->background = 1;
				command->sub_commands[command->num_sub_commands-1].argv[argc] = NULL;
				break;

			case 1 :
				command->stdin_redirect = command->sub_commands[command->num_sub_commands-1].argv[argc+1];
				command->sub_commands[command->num_sub_commands-1].argv[argc] = NULL;
				break;
			
			case 2:
				command->stdout_redirect = command->sub_commands[command->num_sub_commands-1].argv[argc+1];
				command->sub_commands[command->num_sub_commands-1].argv[argc] = NULL;
				break;
				
			default :
				break;

		}
	}
		
}

/*
	divide the command into sub commands
*/
int readArgs(char *in, char **argv, int size)
{
	
	int argc = 0;
	char *delimiter = " ";
	/*Initalize the pointer to avoid the wild pointer*/
	char *seg_str =  NULL;

	if (in == NULL)
	{
		printf("Error Argumnet !\n");
	}

	//first time point the string to be diviede
	seg_str = strtok (in, delimiter);

	for (; seg_str != NULL && argc < size; argc++)
	{
		
		argv[argc] = strdup(seg_str);
		// second time point to the NULL
	 	seg_str = strtok(NULL, delimiter);

	}

	//last argument is NULL
	if(argc == size)
	{
		argv[size-1]= NULL;
	}
	argv[argc] = NULL;

	//argc is the number of arguments in subcomment
	
	// printf ("argument in sting is %d\n", argc);
	// printf ("Last argument in string is %s\n", argv[6]);

	return argc;

}

int env_var_process(struct Subcommand *sub_command)
{
    int i = 0;
    int j = 0;
    
    if(sub_command->line == NULL)
    {
        return -1;
    }
    
    while(sub_command->argv[i] != NULL && i < MAX_ARGS)
    {
        if(strchr(sub_command->argv[i], '=') == NULL)
        {
            break;
        }
        else
        {
            if(i == 0)
                printf("Environment Variables: ");
            printf("%s; ", sub_command->argv[i]);
            putenv(sub_command->argv[i]);
            sub_command->argv[i] = NULL;
        }       
        i++;
    }
    
    if(i > 0)
        printf("\n");
        
    for(; i<MAX_ARGS; i++)
    {
        sub_command->argv[j++] = sub_command->argv[i];
    }
    
    return 0;
}
