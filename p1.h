#ifndef SO_P1_H
#define SO_P1_H

#include "p0.h"

#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <sys/time.h>
#include <stdint.h>
#include <grp.h>
#include <fcntl.h>
#include <pwd.h>


#define MAX_TOKENS 30
#define MAX_PATH 1024

struct stat_options {
    int lon, acc, link;
};

struct list_options{
    struct stat_options statOptions;
    int hid, reca, recb;
};


int create(char *tokens[]);
int stat_fun(char *tokens[]);
int list_fun (char *tokens[]);
int delete(char* tokens[]);
int deltree(char *tokens[]);

/* Funciones auxiliares pero que tambien se usan: */

#endif //SO_P1_H
