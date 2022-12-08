#ifndef SO_P0_H
#define SO_P0_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <sys/utsname.h>

#define MAX_INPUT_SIZE 256
#define MAX_COMANDOS 20

#include "list.h"


int autores(char *tokens[]);
int pid(char *tokens[]);
int carpeta(char *tokens[]);
int fecha(char* tokens[]);
int hist(char* tokens[], list l);
int comando(char *tokens[], list l);
int infosis(char *tokens[]);
int ayuda(char* tokens[]);
int fin(char *tokens[]);

#endif //SO_P0_H
