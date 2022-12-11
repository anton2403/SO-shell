/*  
Carlos Martinez Rabunal , carlos.martinez4@udc.es
Anton Lopez Nunez , anton.lopez.nunez@udc.es
*/

#include "p3.h"

#define _GNU_SOURCE
#define MAXVAR 256
#define MAXNAME 256

extern char **environ;
struct options {
    int a, e, p;
} var_options = {0, 0, 0};


int priority(char *tokens[]){
    
    if (tokens[0] != NULL) { // Hay pid
        int pid = atoi(tokens[0]);

        if (tokens[1] != NULL) { // Hay prioridad
            int prio = atoi(tokens[1]);
            int currentPriority = getpriority(PRIO_PROCESS, 0);
            int returnSet = setpriority(PRIO_PROCESS, pid, prio);

            if (prio < -20 || prio > 20) {
                printf("Error: priority must be between -20 and 20\n");
                return 1;
            } else if(returnSet == -1) {
                perror("Error setting priority");
                return 0;
            } else {
                printf("Priority of process %d changed from %d to %d\n", pid, currentPriority, prio);
                return 0;
            }

        } else { // No hay prioridad
            int currentPriority = getpriority(PRIO_PROCESS, pid);
            printf("Priority of process %d is %d\n", pid, currentPriority);
            return 0;
        }

    } else { // No hay pid
        int currentPriority = getpriority(PRIO_PROCESS, 0);
        printf("Priority of current process (%d) is %d\n", getpid(), currentPriority);
    }
    return 0;
}

int BuscarVariable (char * var, char *e[])  { /*busca una variable en el entorno que se le pasa como parámetro*/

  int pos=0;
  char aux[MAXVAR];
  
  strcpy (aux,var);
  strcat (aux,"=");
  
  while (e[pos]!=NULL)
    if (!strncmp(e[pos],aux,strlen(aux)))
      return (pos);
    else 
      pos++;
  errno=ENOENT;   /*no hay tal variable*/
  return(-1);
}

int CambiarVariable (char * var, char * valor, char *e[]) { /*cambia una variable en el entorno que se le pasa como parámetro lo hace directamente, no usa putenv*/
  int pos;
  char *aux;
   
  if ((pos=BuscarVariable(var,e))==-1)
    return(-1);
 
  if ((aux=(char *)malloc(strlen(var)+strlen(valor)+2))==NULL)
	return -1;
  strcpy(aux,var);
  strcat(aux,"=");
  strcat(aux,valor);
  e[pos]=aux;
  return (pos);
}

int showvar (char *tokens[]) { // Falta acceder por arg3 y environ

    if (tokens[0] != NULL) { // Especifica la variable de la que mostrar informacion
        char *var = tokens[0];
        char *value_getenv = getenv(var);
        
        if (value_getenv != NULL) { // La variable existe
            printf("Con arg3 main %s = %s (%p) \n", var, value_getenv, &value_getenv);
            printf("Con environ %s = %s (%p) \n", var, value_getenv, &value_getenv);
            printf("Con getenv %s = %s (%p) \n", var, value_getenv, &value_getenv);

        } else {  // La variable no existe (getenv devuelve NULL)
            printf("Enviromental Variable '%s' does not exist\n", var);
            perror("");
            return 0;

        } 

    } else { // No especifica que variable mostrar (Listamos todas)
        char **s = environ;

        for (; *s; s++) {
            printf("%s\n", *s);
        }

    }
    return 0;
}

int changevar (char *tokens[]) {

    if (tokens[0] != NULL) { // Hay algo despues del nombre del comando
        char *var;
        char *oldValue;
        // Si el primer argumento es una opcion
        if (strcmp (tokens[0], "-a") == 0) { 
            var_options.a = 1;
            var = tokens[1];
            oldValue = tokens[2];
        } else if (strcmp (tokens[0], "-e") == 0) {
            var_options.e = 1;
            var = tokens[1];
            oldValue = tokens[2];
        } else if (strcmp (tokens[0], "-p") == 0) {
            var_options.p = 1;
            var = tokens[1];
            oldValue = tokens[2];
        } else { // Si el primer argumento es la variable (el segundo sera el nuevo valor)
            var = tokens[0];
            oldValue = getenv(var);

            if (oldValue != NULL) { // La variable existe
                if (tokens[1] != NULL) { // Especifica el valor a asignar (Seugundo valor)
                    char *newValue = tokens[1];
                    int returnSet = setenv(var, newValue, 1);
    
                    if (returnSet == -1) {
                        perror("Error setting variable");
                        return 0;
                    } else {
                        printf("Variable '%s' changed from '%s' to '%s'\n", var, oldValue, newValue);
                        return 0;
                    }
    
                } else { // No especifica el valor a asignar
                    printf("Variable '%s' has value '%s'\n", var, oldValue);
                    return 0;
                }

            } else {  // La variable no existe (getenv devuelve NULL)
                printf("Enviromental Variable '%s' does not exist\n", var);
                perror("");
                return 0;
    
            }
        }
    } else {
        printf("Error: No variable specified\n");
        printf("Usage: changevar [-a|-e|-p] <variable> <value>\n");
        return 0;
    }
}

int showenv (char *tokens[]) {

}

int cmd_fork (char *tokens[], list jobs_list) {
    /*pid_t pid;

    if ((pid=fork())==0) {
        deleteList(jobs_list, free);
        printf ("ejecutando proceso %d\n", getpid());
    } else if (pid!=-1)
        waitpid (pid,NULL,0);*/

    pid_t pid = fork();

    if (pid == 0) {
        deleteList (jobs_list, free);
    } else if (pid < 0) {
        perror("");
    } else if (pid > 0) {
        //Proceso Original
        printf ("ejecutando proceso %d\n", getpid());
        waitpid(pid, NULL, 0); // Suspende la ejecucion del padre hasta que el hijo acabe
    }
}

char *executable (char *tokens[], char *s) {
    char path[MAXNAME];
    static char aux2[MAXNAME];
    struct stat st;
    char *p;
    if (s==NULL || (p=getenv("PATH"))==NULL)
        return s;
    if (s[0]=='/' || !strncmp (s,"./",2) || !strncmp (s,"../",3))
        return s;       /*is an absolute pathname*/
    strncpy (path, p, MAXNAME);
    for (p=strtok(path,":"); p!=NULL; p=strtok(NULL,":")){
        sprintf (aux2,"%s/%s",p,s);
        if (lstat(aux2,&st)!=-1)
            return aux2;
    }
    return s;
}

int listjobs (char *tokens[]) {

}

int deljobs (char *tokens[]) {

}

int job (char *tokens[]) {

}

int cmd_execution (char *tokens[]) {
    
}