#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "list.h"

int priority(char *tokens[]);
int BuscarVariable (char * var, char *e[]);
int CambiarVariable(char * var, char * valor, char *e[]);
int showvar (char *tokens[]);
int changevar (char *tokens[]);
int showenv (char *tokens[]);
int cmd_fork (char *tokens[], list jobs_list);
char *executable (char *tokens[], char *s);
int listjobs (char *tokens[]);
int deljobs (char *tokens[]);
int job (char *tokens[]);
int cmd_execution (char *tokens[]);