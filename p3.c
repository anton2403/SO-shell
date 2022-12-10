/*  
Carlos Martinez Rabunal , carlos.martinez4@udc.es
Anton Lopez Nunez , anton.lopez.nunez@udc.es
*/

#include "p3.h"

#define _GNU_SOURCE
#define MAXVAR 256

extern char **environ;

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
        char *value = getenv(var);

        if (value != NULL) { // La variable existe
            printf("Con arg3 main %s = %s (%p) \n", var, value, &value);
            printf("Con environ %s = %s (%p) \n", var, value, &value);
            printf("Con getenv %s = %s (%p) \n", var, value, &value);

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

int changevar (char *tokens[]) { // Faltan las opciones
    struct options {
        int a, e, p;
    } options = {0, 0, 0};
    
        if (tokens[0] != NULL) { // Especifica la variable a cambiar

            if (strcmp (tokens[0], "-a") == 0) {
                options.a = 1;
            } else if (strcmp (tokens[0], "-e") == 0) {
                options.e = 1;
            } else if (strcmp (tokens[0], "-p") == 0) {
                options.p = 1;
            } else {
                char *var = tokens[0];
                char *oldvalue = getenv(var);

                if (oldvalue != NULL) { // La variable existe
                    if (tokens[1] != NULL) { // Especifica el valor a asignar
                        char *newValue = tokens[1];
                        int returnSet = setenv(var, newValue, 1);
        
                        if (returnSet == -1) {
                            perror("Error setting variable");
                            return 0;
                        } else {
                            printf("Variable '%s' changed from '%s' to '%s'\n", var, oldvalue, newValue);
                            return 0;
                        }
        
                    } else { // No especifica el valor a asignar
                        printf("Variable '%s' has value '%s'\n", var, oldvalue);
                        return 0;
                    }

                } else {  // La variable no existe (getenv devuelve NULL)
                    printf("Enviromental Variable '%s' does not exist\n", var);
                    perror("");
                    return 0;
        
                }
            }
    
        } else { // No especifica que variable cambiar
            printf("Error: No variable specified\n");
            printf("Usage: changevar [-a|-e|-p] <variable> <value>\n");
            return 0;
        }
        return 0;
}

int showenv (char *tokens[]) {

}

int cmd_fork (char *tokens[]) {

    pid_t pid = fork();
    
    if (pid < 0) {
        perror("");
    } else if (pid > 0) {
        //Proceso Original
        printf("Ejecutando proceso %d", pid);
        waitpid(pid, NULL, 0); // Suspende la ejecucion del padre hasta que el hijo acabe
    }
}

int execute (char *tokens[]) {

}

int listjobs (char *tokens[]) {

}

int deljobs (char *tokens[]) {

}

int job (char *tokens[]) {

}

int cmd_execution (char *tokens[]) {
    
}