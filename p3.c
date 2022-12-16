/*  
Carlos Martinez Rabunal , carlos.martinez4@udc.es
Anton Lopez Nunez , anton.lopez.nunez@udc.es
*/
#include "p3.h"

#define _GNU_SOURCE
#define MAXVAR 256
#define MAXNAME 256

extern char **environ;

char * user (uid_t uid){
    struct passwd *p;
    if ((p=getpwuid(uid))!=NULL)
    return p->pw_name;
}

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

int showvar (char *tokens[], char *envp[]) { // Falta acceder por arg3 y environ

    if (tokens[0] != NULL) { // Especifica la variable de la que mostrar informacion
        char *var = tokens[0];
        char *value_getenv = getenv(var);
        int pos = BuscarVariable(tokens[0], envp);
        if(pos >= 0){
            if (envp[pos] != NULL) {
                printf("Con arg3 main %s (%p) %p \n", envp[pos], envp[pos], &envp[pos]);
            } else if (environ[pos] != NULL) {
                printf("Con environ %s = %s (%p) \n", var, environ[pos], &environ[pos]);
            } else if (value_getenv != NULL) { // La variable existe
                printf("Con getenv %s = %s (%p) \n", var, value_getenv, &value_getenv);
            }
        }
        else {  // La variable no existe (getenv devuelve NULL)
            printf("Enviromental Variable '%s' does not exist\n", var);
            perror("");
            return 0;
        }
    }
    else { // No especifica que variable mostrar (Listamos todas)
        char **s = environ;

        for (; *s; s++) {
            printf("%s\n", *s);
        }

    }
    return 0;
}

int changevar (char *tokens[], char *envp[]) {
    if (tokens[0] != NULL && tokens[1] != NULL && tokens[2] != NULL) { // Hay algo despues del nombre del comando
        // Si el primer argumento es una opcion
        if (strcmp(tokens[0], "-a") == 0) {
            CambiarVariable(tokens[1], tokens[2], envp);
        } else if (strcmp(tokens[0], "-e") == 0) {
            CambiarVariable(tokens[1], tokens[2], environ);
        } else if (strcmp(tokens[0], "-p") == 0) {
            char* putenv_arg;
            strcpy(putenv_arg, tokens[1]);
            strcat(putenv_arg, "=");
            strcat(putenv_arg, tokens[2]);
            putenv(putenv_arg);
            //free(putenv_arg);
        } else {
            printf("Error: No variable specified\n");
            printf("Usage: changevar [-a|-e|-p] <variable> <value>\n");
        }
    } else printf("Usage: changevar [-a|-e|-p] var value\n");
    return 0;
}

void aux_showenv(char *envp[], char name_envp) {
    int i = 0;
    while (envp[i] != NULL) {
        printf("%p->%s[%d]=(%p) %s\n", &envp[i], name_envp, i, envp[i], envp[i]);
        i++;
    }
}

int showenv(char *tokens[], char *envp[]) {
    
    if (tokens[0] == NULL) {
        aux_showenv(envp, "main arg3");
    } else {
        if (strcmp(tokens[0], "-environ") == 0) {
            aux_showenv(environ, "environ");
        } else if (strcmp(tokens[0], "-addr") == 0) {
            printf("environ: %p (almacenado en: %p)\n main arg3: %p (almacenado en %p)\n", *environ, &environ, envp,
                   &envp);
        }
        return 0;
    }
}

int cmd_fork(char *tokens[], list jobs_list) {

    pid_t pid = fork();

    if (pid == 0) {
        deleteList(jobs_list, free);
    } else if (pid < 0) {
        perror("");
    } else if (pid > 0) {
        //Proceso Original
        printf("ejecutando proceso %d\n", getpid());
        waitpid(pid, NULL, 0); // Suspende la ejecucion del padre hasta que el hijo acabe
    }
}

void printJob(job_block jb){
    printf("%d \t %d p=%s %s %s %s", jb.pid, jb.prio, jb.date, jb.time, jb.status, jb.name);
}

void printJobsList(list jobs_list){
    for (pos i = first(jobs_list); get(jobs_list, i) != NULL; i = next(jobs_list, i)) {
        job_block *jb = get(jobs_list, i);
        printJob(*jb);
    }
}

void updateJob (job_block *jb, list jobs_list) {

    for (pos i = first(jobs_list); get(jobs_list, i) != NULL ; i = next(jobs_list, i)) {
        job_block *item = get(jobs_list, i);
        if (jb->pid == item->pid) {
            item->status = jb->status;
            item->prio = jb->prio;
            item->date = jb->date;
            item->time = jb->time;
            item->name = jb->name;
        }
    }
}

void updateJobsList(list jobs_list) {
    job_block *item;
    int pid, status;

    for (pos i = first(jobs_list); get(jobs_list, i) != NULL; i = next(jobs_list, i)) {

        item = get(jobs_list, i);

        if (pid == waitpid(pid, &status, WNOHANG | WUNTRACED | WCONTINUED)) { // El proceso cambio de estado
            updateJob(item, jobs_list);
        }
    }
}

int listjobs (char *tokens[], list jobs_list) {
    job_block *item;

    for (pos i = first(jobs_list); get(jobs_list, i) != NULL; i = next(jobs_list, i)) {

        item = get(jobs_list, i);
        printf("%d \t %d p = %s %s %s %s", item->pid, item->prio, item->date, item->time, item->status, item->name);
    }
}

int deljobs (char *tokens[], list jobs_list) {
    updateJobsList(jobs_list);
    // Delete -term o -sign
    if (tokens[0] != NULL && tokens[1] != NULL) {
        if (strcmp(tokens[0], "-term") == 0) {
            // Eliminar todos los procesos terminados
            for (pos i = first(jobs_list); get(jobs_list, i) != NULL; i = next(jobs_list, i)) {
                job_block *item = get(jobs_list, i);
                if (strcmp(item->status, "Terminated") == 0) {
                    deleteAtPosition(jobs_list, i, free);
                }
            }
        } else if (strcmp(tokens[0], "-sign") == 0) {
            //Eliminar todos los procesos terminados por señal
            for (pos i = first(jobs_list); get(jobs_list, i) != NULL; i = next(jobs_list, i)) {
                job_block *item = get(jobs_list, i);
                if (strcmp(item->status, "Signaled") == 0) {
                    deleteAtPosition(jobs_list, i, free);
                }
            }
        } else {
            printf("Usage: deljobs [-term|-sign]\n");
        }
    }      
}

int job (char *tokens[], list l) {
    // imprime el proceso y lo pasa a primer plano (buscamos lista y waitpid) si tiene -algo

    if (tokens[0] != NULL) {
        if (strcmp(tokens[0], "-fg") == 0) {
            // Poner en primer plano
        } else if (strcmp(tokens[0], "-bg") == 0) {
            // Poner en segundo plano
        } else {
            printf("Usage: job [-fg|-bg] <jobid>\n");
        }
    } else {
        printf("Usage: job [-fg|-bg] <jobid>\n");
    }
}

char *Ejecutable(char *s) {
    char path[MAXNAME];
    static char aux2[MAXNAME];
    struct stat st;
    char *p;
    if (s == NULL || (p = getenv("PATH")) == NULL)
        return s;
    if (s[0] == '/' || !strncmp(s, "./", 2) || !strncmp(s, "../", 3))
        return s;       /*is an absolute pathname*/
    strncpy(path, p, MAXNAME);
    for (p = strtok(path, ":"); p != NULL; p = strtok(NULL, ":")) {
        sprintf(aux2, "%s/%s", p, s);
        if (lstat(aux2, &st) != -1)
            return aux2;
    }
    return s;
}

int OurExecvpe( char *file, char *const argv[], char *const envp[]) {
    return (execve(Ejecutable(file), argv, envp));
}

int execute(char* parameters[],int replace, int pri, int wait){
    int pid, pid2;
    pid2= getpid();
    if(replace){
        if(pri)
            setpriority(PRIO_PROCESS,pid2,atoi(parameters[0]));
        if(execvp(parameters[0], &parameters[0]) == -1){
            perror("Error");
            return 0;
        }
    } else if ((pid=fork())==0){
        if(pri){
            pid2=getpid();
            setpriority(PRIO_PROCESS,pid2,atoi(parameters[0]));
        }
        if(execvp(parameters[pri], &parameters[pri])==-1){
            perror("Error");
            exit(0);
            return 0;
        }
    }
    if(wait)
        waitpid (pid,NULL,0);
    return pid;
}

int cmd_execution(char *tokens[], char *arvg[], char * envp[]) {
    char priority[256];
    char *priority_string;
    int amp, arr, prio;
    char *command[100];
    command[0] = tokens[0];
    

    for (int i = 1; tokens[i] !=NULL; i++) { // Recorremos el array para ver si tiene el ampersand (Esta al final, pero no sabemos que parametros se le envian)
        if (strncmp(tokens[i], "-", 1 ) == 0 && strncmp(tokens[i], "&", 1 ) != 0 && strncmp(tokens[i], "@", 1 ) != 0 && tokens[i] != NULL) {
            command[i] = tokens[i];
        } else if (strcmp(tokens[i], "&")==0) {
            amp = 1;
        } else if (strncmp(tokens[i], "@", 1) == 0) {
            arr = 1;
            strcpy(priority, tokens[i]);
            prio = (int) strtol(memmove(&priority[0], &priority[1], 256), NULL, 10);
            printf("prio: %d\n", prio);
            // We need to remove the priority from the tokens array
            tokens[i] = tokens[i+1];
        }
    }
    OurExecvpe(*command, arvg, envp);
}