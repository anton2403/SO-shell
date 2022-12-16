#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "list.h"

#define FINISHED 0
#define STOPPED 1
#define SIGNALED 2
#define ACTIVE 3

typedef struct jblock {
    char* name;
    pid_t pid;
    char *date;
    char *time;
    char* status;
    int senal;
    int prio;
    int command_line;
} job_block;

int priority(char *tokens[]);
//int BuscarVariable (char * var, char *e[]);
//int CambiarVariable(char * var, char * valor, char *e[]);
int showvar (char *tokens[], char* envp[]);
int changevar (char *tokens[], char* envp[]);
int showenv (char *tokens[], char* envp[]);
int cmd_fork (char *tokens[], list jobs_list);
//char *executable (char *tokens[], char *s);
int OurExecvpe ( char *file, char *const argv[], char *const envp[]);
int execute(char* parameters[],int replace, int pri, int wait);
int listjobs (char *tokens[], list l);
int deljobs (char *tokens[], list l);
int job (char *tokens[], list l);
int cmd_execution(char *tokens[], char *arvg[], char * envp[]);