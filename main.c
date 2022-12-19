
/*
Carlos Martinez Rabunal , carlos.martinez4@udc.es
Anton Lopez Nunez , anton.lopez.nunez@udc.es
*/

#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

/* P0 ***************************************************************************************************************** */
#include "p0.h"
/* P1 ***************************************************************************************************************** */
#include "p1.h"
/* ******************************************************************************************************************** */
#include "p2.h"
/* ******************************************************************************************************************** */
#include "p3.h"

#define MAX_INPUT_SIZE 256
#define MAX_TOKENS 30
#define MAX_COMANDOS 20
#define MAX_CMD 4096

typedef struct lists {
    list hist_list;
    list mem_list;
    list jobs_list;
} lists;


/* Functions Declaration *********************************************************************************************** */

int process_input(char *tokens[], struct lists listas, char *envp[]);
int split_string(char* string, char* trozos[]);

/* P0 **************************************************************************************************************** */

int comando(char *tokens[], list l, char *envp[]){
    lists lists;
    lists.mem_list = l;
    int num_comand = atoi(tokens[0]);
    int counter = 1;
    pos i = NULL;
    char aux[MAX_INPUT_SIZE];

    if(*tokens == NULL){
    	return 0;
    }

    for (i = first(l);(counter < num_comand ) && !at_end(l, i)  ; i = next(l, i)){
        counter = counter + 1;
    }
    strcpy(aux, get(l, i));
    printf("%d) %s \n",counter, aux);
    strcpy(tokens[0], aux);
    split_string(aux, tokens);
    process_input(tokens, lists, envp);
    return 0;
}

/* P1 **************************************************************************************************************** */

/* P2 **************************************************************************************************************** */

/* ******************************************************************************************************************* */

int split_string(char * string, char * trozos[]){
    int i=1;
    if ((trozos[0]=strtok(string," \n\t") )==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}

struct cmd{  //ARRAY A UNA FUNCION
    char *cmd_name;
    int (*cmd_fun)(char *tokens[]);
}
cmds[MAX_CMD]= {//Variable global. Array de comandos. NOMBRE / FUNCION
        {"autores", autores},
        {"pid", pid},
        {"carpeta", carpeta},
        {"fecha",fecha},
        //hist es una funcion a parte
        //comando es una funcion a parte
        {"infosis",infosis},
        {"ayuda", ayuda},
        {"help", ayuda},
        {"fin", fin},
        {"salir", fin},
        {"bye", fin},
        {"exit", fin},
        {"create", create},
        {"stat", stat_fun},
        {"list", list_fun},
        {"delete", delete},
        {"deltree", deltree},
        //allocate es una funcion a parte
        //deallocate es una funcion a parte
        //memory es una funcion a parte
        {"i-o", io},
        {"recurse", recurse},
        {"memfill", memfill},
        {"memdump", memdump},
        {"prioridad", priority},
        // showvar es una funcion a parte
        // changevar es una funcion a parte
        // showenv es una funcion a parte
        // fork es una funcion a parte
        // job, deljob y listjob son funciones a parte
        {"execute", execute},
        {NULL, NULL}
        };

int process_input(char *tokens[], lists lists, char* envp[]){
    int i;
    //El hist es un comando que no esta en el array cmds[] porque necesita acceder a la lista
    if(*tokens != NULL){
        if (strcmp(tokens[0], "hist") == 0)
            return hist(tokens + 1, lists.hist_list);
        else if (strcmp(tokens[0], "comando") == 0)
            return comando(tokens + 1, lists.hist_list, envp);
        else if(strcmp(tokens[0], "allocate") == 0)
            return allocate(tokens+1, lists.mem_list);
        else if(strcmp(tokens[0], "deallocate") == 0)
            return deallocate(tokens+1, lists.mem_list);
        else if (strcmp(tokens[0], "memory") == 0)
            return memory(tokens+1, lists.mem_list);
        else if (strcmp(tokens[0], "showvar") ==0)
            return showvar(tokens +1, envp);
        else if (strcmp(tokens[0], "changevar")==0)
            return changevar(tokens + 1, envp);
        else if (strcmp (tokens[0], "showenv")==0)
            return showenv(tokens +1, envp);
        else if (strcmp(tokens[0], "fork") == 0)
            return cmd_fork(tokens +1, lists.jobs_list);
        else if (strcmp(tokens[0], "listjobs")==0)
            return listjobs(tokens, lists.jobs_list);
        else if (strcmp(tokens[0], "deljobs")==0)
            return listjobs(tokens, lists.jobs_list);
        else if (strcmp(tokens[0], "job")==0)
            return listjobs(tokens, lists.jobs_list);

        else {
            for (i = 0; cmds[i].cmd_name != NULL; i++) {
                if (strcmp(tokens[0], cmds[i].cmd_name) == 0) {
                    return cmds[i].cmd_fun(tokens + 1);
                }
            }
        }
        //SI LLEGO AQUI ES QUE NO ENCONTRO EL COMANDO
        cmd_execution(tokens, envp, envp);
        
    }
    return 0;
}

int main(int argc, char *arvg[], char *envp[]) {
    lists lists;
    pos i;
    
    create_list(&lists.hist_list);
    create_list(&lists.mem_list);
    create_list(&lists.jobs_list);

    char input[MAX_INPUT_SIZE];
    char *tokens[MAX_TOKENS]; //Array de punteros
    int end=0;
    char* str;
    mem_block *aux;

    while(!end){
        printf(">");
        fgets(input, MAX_INPUT_SIZE, stdin);
        str = strdup(input);
        InsertElement(lists.hist_list,str);

        split_string(input, tokens);
        end = process_input(tokens, lists, envp);
        }
    deleteList(lists.hist_list, free);
    for(i = first(lists.mem_list); !at_end(lists.mem_list, i); i = next(lists.mem_list,i)){
        aux = get(lists.mem_list, i);
        free(aux->mem_addr);
    }
    deleteList(lists.mem_list, free);
    free(lists.mem_list);
    free(lists.hist_list);
    return 0;
}