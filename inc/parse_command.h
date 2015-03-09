#ifndef PARSECOMMAND_H_
#define PARSECOMMAND_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>	/* needed to define getpid() */
#include <stdbool.h>
#define MAX_SUB_COMMANDS 50
#define MAX_ARGS 10000

struct Subcommand
{
	char *line;
	char *argv[MAX_ARGS];
};

struct Command
{
	struct Subcommand sub_commands[MAX_SUB_COMMANDS];
	int num_sub_commands;
	char *stdin_redirect ;
	char *stdout_redirect;
	int background ;
};

void readCommand(char *, struct Command *);

int stringCmp (char *);

void readRedirectAndBackground(struct Command *);

int readArgs(char *, char **, int );

int env_var_process(struct Subcommand *);
#endif