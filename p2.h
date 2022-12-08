#ifndef SO_P2_H
#define SO_P2_H

#include "list.h"
#include "p0.h"

#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <error.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <ctype.h>
#include <sys/mman.h>


#define TAMANO 2048

#define NULLKEY (-1)
#define NULLFD (-1)


#define MALLOC 0
#define SHARED 1
#define MMAP 2

typedef struct mem_block {
    key_t key;
    void * mem_addr;
    size_t size;
    char time[256];
    int type_alloc;
    char name[MAX_INPUT_SIZE];
    int fd;
} mem_block;

typedef struct alloc_options{
    int malloc;
    int shared;
    int mmap;
}alloc_options;

int allocate(char *tokens[], list l);
int deallocate(char *tokens[], list l);
int io(char *tokens[]);//lista
int memdump(char *tokens[]);//lista
int memfill (char *tokens[]);//lista
int memory(char *tokens[], list l);//lista
int recurse (char* tokens[]);//lista

void delete_data(void*);

#endif //SO_P2_H