/*  
Carlos Martinez Rabunal , carlos.martinez4@udc.es
Anton Lopez Nunez , anton.lopez.nunez@udc.es
*/

#include "p0.h"

int autores(char *tokens[]) {
    /* Autores: */
    struct autor{
        char name[25];
        char login[25];
    };
    struct autor carlos = {"Carlos Martinez Rabunal","carlos.martinez4@udc.es"};
    struct autor anton = {"Anton Lopez Nunez","anton.lopez.nunez@udc.es"};

    if(*tokens==NULL)
        printf("%s: %s\n%s: %s\n", anton.name,anton.login,carlos.name,carlos.login);
    else if((strcmp(tokens[0],"-n")==0) && (tokens[1]==NULL))
        printf("%s\n%s\n", anton.name, carlos.name);
    else if((strcmp(tokens[0],"-l")==0) && (tokens[1]==NULL))
        printf("%s\n%s\n", anton.login, carlos.login);
    else
        puts("Comando not found");

    return 0;
}

int pid(char *tokens[]){
    if(*tokens==NULL)
        printf("Pid of the shell: %d\n",getpid());
    else if((strcmp(tokens[0],"-p")==0) && (tokens[1]==NULL))
        printf("Pid of the shell parent's process: %d\n", getppid());
    else
        puts("Comand not found");
    return 0;
}

int carpeta(char *tokens[]){
    char s[MAX_INPUT_SIZE];
    if(*tokens==NULL){
        getcwd(s, MAX_INPUT_SIZE);
        printf("%s\n", s);
    }
    else if ((chdir(tokens[0]) == 0) && (tokens[1]==NULL)){
        getcwd(s, MAX_INPUT_SIZE);
        printf("Now you are in %s\n", s);
    }
    else{
        puts("Directory not found");
    }
    return 0;
}
int fecha(char* tokens[]){
    time_t rawtime;
    struct tm *info;
    char date[MAX_INPUT_SIZE];
    char hour[MAX_INPUT_SIZE];

    time( &rawtime );

    info = localtime( &rawtime );

    strftime(date,MAX_INPUT_SIZE,"%d/%m/%Y", info);
    strftime(hour,MAX_INPUT_SIZE,"%H:%M:%S", info);

    if(*tokens==NULL)
        printf("Date and Time : %s %s\n", date, hour );
    else if ((strcmp(tokens[0],"-d")==0) && (tokens[1]==NULL))
        printf("Date: %s \n", date);
    else if((strcmp(tokens[0],"-h")==0) && (tokens[1]==NULL))
        printf("Time : %s \n", hour);
    else
        puts("Comand not found");

    return 0;
}
int hist(char* tokens[], list l){
    char aux[MAX_INPUT_SIZE];
    int i = 1;

    if(*tokens==NULL){
        for (pos p = first(l); !at_end(l, p); p = next(l, p)) {
            strcpy(aux, get(l,p));
            printf("%d) %s", i,aux);
            i++;
        }
    }
    else if((strcmp(tokens[0],"-c")==0) && (tokens[1]==NULL)) {
        deleteList(l,free);
        if(isEmptyList(l))
            puts("The comands history has been deleted\n");
        else
            puts("Something has gone wrong\n");

    }

    else if((atoi(tokens[0]) <= 0) && (tokens[1]==NULL)){
        int k = abs(atoi(tokens[0]));
        for (pos p = first(l); !at_end(l, p) && (i<=k && k<= MAX_COMANDOS) ; p = next(l, p)) {
            strcpy(aux, get(l,p));
            printf("%d) %s", i,aux);
            i++;
        }
    }
    else
        puts("Comand not found");
    return 0;
}



int infosis(char *tokens[]){
    struct utsname my_uname;
    if(uname(&my_uname) == -1)
        printf("Infosis call failed!");
    else
        printf("Nodename: %s\nMachine: %s\nSystem name: %s\nRelease: %s\nVersion: %s\n",
               my_uname.nodename, my_uname.machine, my_uname.sysname, my_uname.release,my_uname.version);
    return 0;
}

int ayuda(char* tokens[]){
    if(*tokens == NULL){
        printf("AVAILABLE COMMANDS\n"
               "\tautores\n"
               "\tpid\n"
               "\tcarpeta\n"
               "\tborrarrec\n"
               "\tbye\n"
               "\tcarpeta\n"
               "\tfecha\n"
               "\thist\n"
               "\tcomando\n"
               "\tinfosis\n"
               "\tayuda\n"
               "\tcreate\n"
               "\tstat\n"
               "\tfin\n"
               "\tallocate\n"
               "\tdeallocate\n"
               "\tio\n"
               "\tmemdump\n"
               "\tmemfill\n"
               "\tmemory\n"
               "\trecurse\n"
               "\tprioridad\n"
               "\tshowvar\n"
               "\tchangevar\n"
               "\tshowenv\n"
               "\tfork\n"
               "\texecute\n"
               "\tlistjobs\n"
               "\tdeljobs\n"
               "\tjob\n"
               "\t*****\n"
               "\t\n");
    } else if(strcmp(tokens[0], "autores") == 0){
        printf("NAME\n\tautores\n"
               "SYNTAX\n\tautores [-l | -n]\n"
               "DESCRIPTION\n\tPrints the names and logins of the program authors.\n"
               "OPTIONS\n"
               "\t-l     prints only the logins.\n"
               "\t-n     prints only the names.\n");
    } else if(strcmp(tokens[0], "pid") == 0){
        printf("NAME\n\tpid\n"
               "SYNTAX\n\tpid [-p]\n"
               "DESCRIPTION\n\tPrints the pid of the process executing the shell.\n"
               "OPTIONS\n"
               "\t-p     Prints the pid of the shell's parent process.\n");
    } else if(strcmp(tokens[0], "carpeta") == 0){
        printf("NAME\n\tcarpeta\n"
               "SYNTAX\n\tcarpeta [direct]\n"
               "DESCRIPTION\n\tPrints the current working directory or, if the \"direct\" is given, changes it to \"direct\".\n");
    } else if(strcmp(tokens[0], "fecha") == 0){
        printf("NAME\n\tfecha\n"
               "SYNTAX\n\tfecha [-d |-h]\n"
               "DESCRIPTION\n\tPrints the current system date and time.\n"
               "OPTIONS\n"
               "\t-d     prints the current date in format DD/MM/YYYY.\n"
               "\t-h     prints the current time in format hh:mm:ss.\n");
    } else if(strcmp(tokens[0], "hist") == 0){
        printf("NAME\n\thist\n"
               "SYNTAX\n\thist [-c|-N]\n"
               "DESCRIPTION\n\tShows/clears the historic of comands. Without arguments prints all the commands in order.\n"
               "OPTIONS\n"
               "\t-c     clear the history.\n"
               "\t-N     print the first N commands.\n");
    } else if(strcmp(tokens[0], "comando") == 0){
        printf("NAME\n\tcomando\n"
               "SYNTAX\n\tcomando N\n"
               "DESCRIPTION\n\tRepeat the command number N.\n");
    } else if(strcmp(tokens[0], "infosis") == 0){
        printf("NAME\n\tinfosis\n"
               "SYNTAX\n\tinfosis\n"
               "DESCRIPTION\n\tPrints information of the machine running the shell.\n");
    } else if(strcmp(tokens[0], "ayuda") == 0) {
        printf("You can get information of all this commands:\n"
               "\tautores\n"
               "\tpid\n"
               "\tcarpeta\n"
               "\tfecha\n"
               "\thist\n"
               "\tcomando\n"
               "\tinfosis\n"
               "\tfin / salir / bye\n");
    } else if (strcmp(tokens[0], "create") == 0) {
        printf("NAME\n"
               "\tcreate\n"
               "SYNTAX\n"
               "\tcreate [-f] [name]\n"
               "DESCRIPTION\n"
               "\tCreates a directory or file (-f)\n"
               "OPTIONS\n"
               "\t-f Creates a file\n");
    } else if (strcmp(tokens[0], "stat") == 0) {
        printf("NAME\n"
               "\tstat\n"
               "SYNTAX\n"
               "\tstat [-long][-link][-acc] name1 name2\tlists files\n"
               "DESCRIPTION\n"
               "\tGives information on files or directories\n"
               "OPTIONS\n"
               "\t-long: long list.\n"
               "\t-acc: acesstime.\n"
               "\t-link: the path of the symbolic link.\n");
    } else if(((strcmp(tokens[0], "fin") == 0) || (strcmp(tokens[0], "bye") == 0) || strcmp(tokens[0], "salir") == 0)){
        printf("NAME\n"
               "\tfin, salir, bye\n"
               "SYNTAX\n"
               "\tfin\n\tsalir\n\tbye\n"
               "DESCRIPTION\n"
               "\tEnds the shell.\n");
    } else if (strcmp(tokens[0], "allocate") == 0) { // p2
        printf("NAME\n"
               "\tallocate\n"
               "SYNTAX\n"
               "\tallocate [-malloc|-shared|-createshared|-mmap]\n"
               "DESCRIPTION\n"
               "\tAllocates a memory block\n"
               "OPTIONS\n"
               "\t-malloc tam\n"
               "\t\tAllocates a malloc block of size tam\n"
               "\t-createshared cl tam\n"
               "\t\tAllocates (creating it) a shared memory block of size tam and key cl\n"
               "\t-shared cl\n"
               "\t\tAllocates a shared memory block (already created) of size tam and cl key\n"
               "\t-mmap fich perm\n"
               "\t\tMaps fich file, perm are permissions\n");
    } else if (strcmp(tokens[0], "deallocate") == 0) {
        printf("NAME\n"
               "\tdeallocate\n"
               "SYNTAX\n"
               "\tdeallocate [-malloc|-shared|-createshared|-mmap|addr]\n"
               "DESCRIPTION\n"
               "\tDeallocates a memory block\n"
               "OPTIONS\n"
               "\t-malloc tam\n"
               "\t\tDeallocates the malloc block of size tam\n"
               "\t-shared cl\n"
               "\t\tDeallocates (unmaps)the shared memory block with key cl\n"
               "\t-delkey cl\n"
               "\t\tRemoves from the system (without unmapping) the memory key cl\n"
               "\t-mmap fich\n"
               "\t\tUnmaps mapped file fich\n"
               "\taddr: Unallocates the memory block at the addr addres\n");
    } else if (strcmp(tokens[0], "io") == 0 || strcmp(tokens[0], "i-o") == 0) {
        printf("NAME\n"
               "\ti-o\n"
               "SYNTAX\n"
               "\ti-o [read|write] [-o] fiche addr cont\n"
               "DESCRIPTION\n"
               "\tPerforms input-output actions\n"
               "OPTIONS\n"
               "\tread fich addr cont\n"
               "\t\tReads cont bytes from fich to addr\n"
               "\twrite [o]\n"
               "\t\tWrites cont bytes from addr to fich, -o to overwrite\n"
               "\taddr: it's a memory address\n");
    } else if (strcmp(tokens[0], "memdump") == 0) {
        printf("NAME\n"
               "\tmemdump\n"
               "SYNTAX\n"
               "\tmemdump addr [cont]\n"
               "DESCRIPTION\n"
               "\tShows the contents of cont bytes starting at memory address addr. If cont is not specified, show "
               "25 bytes. For each byte, print its hex value and its associated char, at 25 bytes per line.\n");
    } else if (strcmp(tokens[0], "memfill") == 0) {
        printf("NAME\n"
                   "\tmemfill\n"
                   "SYNTAX\n"
                   "\tmemfill addr [cont] [byte]\n"
                   "DESCRIPTION\n"
                   "\tFill cont bytes starting at address addr with value byte. If byte is not specified, assume a "
                   "value of A(65). If cont is not specified, use a default value of 128.\n");
    } else if (strcmp(tokens[0], "memory") == 0) {
        printf("NAME\n"
                   "\tmemory\n"
                   "SYNTAX\n"
                   "\tmemory [-blocks | -vars | -funcs | -all | -pmap]\n"
                   "DESCRIPTION\n"
                   "\tShow addresses inside the process memory space. If no argument is given, equivalent to -all.\n"
                   "OPTIONS\n"
                   "\t-bocks\n"
                   "\t\tShow the list of addresses allocated with the malloc, shared and mmap.\n"
                   "\t-vars\n"
                   "\t\tPrint the memory addresses of nine variables of the shell: three external, three static, and "
                   "three automatic initialized variables.\n"
                   "\t-funcs\n"
                   "\t\tPrint the memory addresses of the three program functions of the shell and three C library "
                   "functions used in the shell program.\n"
                   "\t-all\n"
                   "\t\tThe equivalent to -blocks, -vars, and -funcs combined.\n"
                   "\t-pmap\n"
                   "\t\tCall the program pmap for the shell process.\n");
    } else if (strcmp(tokens[0], "recurse") == 0) {
        printf("NAME\n"
                   "\trecurse\n"
                   "SYNTAX\n"
                   "\trecurse [n]\n"
                   "DESCRIPTION\n"
                   "\tCall a recursive function passing the integer n as its parameter. It receives the number of "
                   "times it has to call itself. This function has two variables: an automatic array of 4096 bytes "
                   "and a static array of the same size.\n"
                   "\tThis function prints the value of the received parameter as well as its memory address, the "
                   "address of the static array and the automatic array, decrements n and if n > 0, calls itself.\n");
    } else if (strcmp(tokens[0], "prioridad") == 0) {
        printf("NAME\n"
                   "\tprioridad\n"
                   "SYNTAX\n"
                   "\tprioridad [pid] [valor prioridad]\n"
                   "DESCRIPTION\n"
                   "\tShows or changes a process's priority.\n");
    } else if (strcmp(tokens[0], "showvar") == 0) {
        printf("NAME\n"
                   "\tshowvar\n"
                   "SYNTAX\n"
                   "\tshowvar [var]\n"
                   "DESCRIPTION\n"
                   "\tShows the value and address of a enviromental variable.\n");
    } else if (strcmp(tokens[0], "changevar") == 0) {
        printf("NAME\n"
                   "\tcambiarvar\n"
                   "SYNTAX\n"
                   "\tcambiarvar [-a | -e | -p] var valor\n"
                   "DESCRIPTION\n"
                   "\tChanges value of a enviromental variable\n"
                   "OPTIONS\n"
                   "\t-a\n"
                   "\t\tAccede por el tercer arg de main.\n"
                   "\t-e\n"
                   "\t\tAccede mediante environ\n"
                   "\t-p\n"
                   "\t\tAccede mediante putenv.\n");
    } else if (strcmp(tokens[0], "showenv") == 0) { // INCOMPLETO
        printf("NAME\n"
                   "\tshowenv\n"
                   "SYNTAX\n"
                   "\tshowenv [-enviromc | -addr]\n"
                   "DESCRIPTION\n"
                   "\tShows the process enviroment\n"
                   "OPTIONS\n"
                   "\t-envirom\n"
                   "\t\t.\n"
                   "\t-addr\n"
                   "\t\t\n");
    } else if (strcmp(tokens[0], "fork") == 0) {
        printf("NAME\n"
                   "\tfork\n"
                   "SYNTAX\n"
                   "\tfork\n"
                   "DESCRIPTION\n"
                   "\tMakes a fork system call to create a proccess\n");
    } else if (strcmp(tokens[0], "execute") == 0) {
        printf("NAME\n"
                   "\texecute\n"
                   "SYNTAX\n"
                   "\texecute prog arg...\n"
                   "DESCRIPTION\n"
                   "\tExecutes, without creating the proccess, prog with arguments\n");
    } else if (strcmp(tokens[0], "listjobs") == 0) {
        printf("NAME\n"
                   "\tlistjobs\n"
                   "SYNTAX\n"
                   "\tlistjobs\n"
                   "DESCRIPTION\n"
                   "\tLists background proccess\n");
    } else if (strcmp(tokens[0], "deljobs") == 0) {
        printf("NAME\n"
                   "\tdeljobs\n"
                   "SYNTAX\n"
                   "\tdeljobs [-term] [-sig]\n"
                   "DESCRIPTION\n"
                   "\tDeletes all proccess from listjobs in sp\n"
                   "OPTIONS\n"
                   "\t-term\n"
                   "\t\tThe finished ones.\n"
                   "\t-sig\n"
                   "\t\tThe ones finished by signal\n");
    } else if (strcmp(tokens[0], "job") == 0) {
        printf("NAME\n"
                   "\tjob\n"
                   "SYNTAX\n"
                   "\tjob [-fg] pid\n"
                   "DESCRIPTION\n"
                   "\tShows information about proccess pid\n"
                   "OPTIONS\n"
                   "\t-fg\n"
                   "\t\tMoves it to foreground.\n");
    } else if (strcmp(tokens[0], "*****") == 0) {
        printf("NAME\n"
                   "\t*****\n"
                   "SYNTAX\n"
                   "\t[VAR1 VAR2 .....] executable arg1 arg2......[@pri] [&]\n"
                   "DESCRIPTION\n"
                   "\tFor anything that is not a shell command, the shell will asume it is an external program (in the format for execution described below) and will create a process to execute it)\n"
                   "OPTIONS\n"
                   "\t[VAR1 VAR2...]\n"
                   "\t\tIf present, means that the program being executed has an environment"
                    "consisting only of the variables VAR1, VAR2...(VAR1, VAR2 are the names of the variables,"
                    "their values are taken from environ in the current process). The number of variables is"
                    "undefined and they need not be capitalized.\n"
                   "\texecutable\n"
                   "\t\tIs the name of the executable file to be executed.\n"
                   "\targ1 arg2 ...\n"
                   "\t\tAre the arguments passed to the executable. (number of them is undefined).\n"
                   "\t[@pri]\n"
                   "\t\tif present, means that the new program being executed has its priority set (with setpriority) to pri.\n"
                   "\t[&]\n"
                   "\t\tIf present, means that execution is to be in the background.\n"
                   "\t\tThis format, with the execption of the &, is the same to be used with the execute command to execute without creating process.\n");
    } else
        puts("The information of that command has not been found");

    return 0;
}

int fin(char *tokens[]){
    printf("The shell has finished\n");
    return 1;
}