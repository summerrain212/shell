#ifndef EXECUTE_H_
#define EXECUTE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>	/* needed to define getpid() */
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <errno.h>

#include "parse_command.h"

int executeCommand(struct Command *);

bool is_path (char* );

char* pathFormatOperation(char *); 

#endif /* EXECUTE__H_ */