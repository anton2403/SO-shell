/*  
Carlos Martinez Rabunal , carlos.martinez4@udc.es
Anton Lopez Nunez , anton.lopez.nunez@udc.es
*/

#include "p2.h"

/* ******************************************************************************************************************* */
//GLOBAL VARS
char global1 = 'a';
int global2 = 1;
float global3 = (float) 1.5;

/* STRUCT FUNCTIONS ************************************************************************************************** */

void emptyoptions(alloc_options *allocOptions) {
    allocOptions->shared = 0;
    allocOptions->malloc = 0;
    allocOptions->mmap = 0;
}

void alloptions(alloc_options *allocOptions){
    allocOptions->malloc = 1;
    allocOptions->shared = 1;
    allocOptions->mmap = 1;
}

/* STRING TO POINTER ************************************************************************************************* */
void * cadtop(char * str) {
    long int  aux = strtoul(str, NULL, 16);
    return (void *) (uintptr_t)aux;
}

/* PRINT FUNCTIONS *************************************************************************************************** */
void printvariables(){
    char local1 = '7';
    int local2 = 45;
    float local3 = (float) 1.4;
    static char static1 = 'a';
    static int static2 = 1;
    static float static3 = (float) 2.5;
    printf("%-30s\t%p, \t%p, \t%p\n", "Variables locales:\n", &local1, &local2, &local3);
    printf("%-30s\t%p, \t%p, \t%p\n", "Variables globales:\n",&global1, &global2, &global3);
    printf("%-30s\t%p, \t%p, \t%p\n", "Variables estaticas:\n",&static1, &static2, &static3);
}

int funct1(){
    return 0;
}

char funct2(){
    return 'a';
}

double funct3(){
    return 5.8;
}

void printfuncs(){
    printf("%-30s\t%p, \t%p, \t%p\n", "Funciones programa:", funct1, funct2, funct3);
    printf("%-30s\t%p, \t%p, \t%p\n", "Funciones libreria:", strcmp, deleteAtPosition, getpid);
}

char* getTypeAlloc(int a){
    if(a == MALLOC)
        return"malloc";
    else if(a == SHARED)
        return "shared";
    else if (a == MMAP)
        return "mmap";
    else
        return "other";
}

void printMemList(list l, alloc_options allocOptions) {
    pos i;
    mem_block *aux;
    printf("***Lista de bloques asignados al proceso %d:\n", getpid());
    if(isEmptyList(l)){

    }
    else {
        if(allocOptions.malloc && allocOptions.shared && allocOptions.mmap){
            for(i = first(l); !at_end(l,i); i = next(l,i)){
                aux = get(l,i);
                printf("direction: %p size: %zu time: %s \t %s \n", aux->mem_addr, aux->size, aux->time,getTypeAlloc(aux->type_alloc));
            }
        }
        else if(allocOptions.malloc){
            for(i = first(l); next(l,i) != NULL; i = next(l,i)){
                aux = get(l,i);
                if(aux->type_alloc == MALLOC)
                    printf("direction: %p size: %zu %s malloc\n", aux->mem_addr, aux->size, aux->time);
            }
        }
        else if(allocOptions.shared){
            for(i = first(l); !at_end(l,i); i = next(l,i)){
                aux = get(l,i);
                if(aux->type_alloc == SHARED)
                    printf("direction: %p size: %zu %s shared\n", aux->mem_addr, aux->size, aux->time);
            }
        }
        else if(allocOptions.mmap) {
            for (i = first(l); !at_end(l, i); i = next(l, i)) {
                aux = get(l, i);
                if (aux->type_alloc == MMAP)
                    printf("direction: %p size: %zu %s mmap\n", aux->mem_addr, aux->size, aux->time);
            }
        }
    }
}
/* MEM LIST FUNCTIONS ************************************************************************************************ */
void gettime(char* thetime) {
    time_t rawtime;
    struct tm *info;
    char date[MAX_INPUT_SIZE];
    char hour[MAX_INPUT_SIZE];

    time( &rawtime );

    info = localtime( &rawtime );

    strftime(date,MAX_INPUT_SIZE,"%d/%m/%Y", info);
    strftime(hour,MAX_INPUT_SIZE,"%H:%M:%S", info);

    strcat(date, "\t");
    strcat(date, hour);
    strcpy(thetime,date);
}

void* auxcreateMem_block(key_t key, void* mem_addr, size_t size, int type_alloc, char* name, int fd) {
    mem_block  *memBlock;
    char thetime[MAX_INPUT_SIZE];
    memBlock = malloc(sizeof(struct mem_block));

    strcpy(memBlock->name, name);
    gettime(thetime);
    strcpy(memBlock->time, thetime);
    memBlock->mem_addr = mem_addr;
    memBlock->size = size;
    memBlock->type_alloc = type_alloc;
    memBlock->key = key;
    memBlock->fd = fd;
    return memBlock;
}
/* ******************************************************************************************************************* */
void freedata(void *p){
    mem_block *memblock;
    memblock = p;
    switch(memblock->type_alloc){
        case MALLOC: free(p);
        break;
        case SHARED: shmdt(p);
        break;
        case MMAP:{
            munmap(p, memblock->size);
            close(memblock->fd);
        }
        break;
        default: printf("Imposible eliminar %p", p);
    }
}
/* ******************************************************************************************************************* */
void * ObtenerMemoriaShmget (key_t clave, size_t tam, list mem_list) {
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;

    if (tam) //tam distito de 0 indica crear
                flags=flags | IPC_CREAT | IPC_EXCL;
    if (clave==IPC_PRIVATE) {
        errno=EINVAL; return NULL;
    }
    if ((id=shmget(clave, tam, flags))==-1)
        return (NULL);
    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno;
        if (tam)
            shmctl(id,IPC_RMID,NULL);
        errno=aux;
        return (NULL);
    }
    shmctl (id,IPC_STAT,&s);
    //InsertElement(mem_list, auxcreateMem_block(clave,p,s.shm_segsz,SHARED,"shared", NULLFD));
    return (p);
}

/*void do_AllocateShared(key_t cl, list mem_list){
    size_t tam;
    void *p;

    tam=(size_t) (-1);
    if ((p=ObtenerMemoriaShmget(cl,tam, mem_list))!=NULL){
        printf ("Memoria compartida de clave %d en %p\n", cl, p);
        InsertElement(mem_list, auxcreateMem_block(cl,p,tam,SHARED,"shared", NULLFD));
    }
    else
        printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}*/

void do_AllocateCreateshared (char *key, char* size, list mem_list) {
    alloc_options allocOptions;
    emptyoptions(&allocOptions);
    key_t cl;
    size_t tam;
    void *p;

    if (key == NULL || size == NULL) {
        printMemList(mem_list, allocOptions);
        return;
    }
    /*if(isUsedKey(mem_list, *key)){
        printf("The key is used\n");
        return;
    }*/

    cl=(key_t)  strtoul(key,NULL,10);
    tam=(size_t) strtoul(size,NULL,10);
    if (tam==0) {
        printf ("No se asignan bloques de 0 bytes\n");
        return;
    }
    if ((p=ObtenerMemoriaShmget(cl,tam, mem_list))!=NULL){
        printf ("Asignados %lu bytes en %p\n",(unsigned long) tam, p);
        InsertElement(mem_list, auxcreateMem_block(cl,p,tam,SHARED,"shared", NULLFD));
    }
    else
        printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}

void * mapearFichero (char * fichero, int protection, list l) {
    int df, map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    void *p;

    if (protection&PROT_WRITE)
        modo=O_RDWR;
    if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
        return NULL;
    if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)
        return NULL;
/* Guardar en la lista    InsertarNodoMmap (&L,p, s.st_size,df,fichero); */
    printf("%s file mapped in %p\n",fichero, p );
    InsertElement(l, auxcreateMem_block(NULLKEY,p,s.st_size,MMAP,fichero, df));

    return p;
}

/* I-O AUXILIARY FUNCTIONS ******************************************************************************************* */
ssize_t LeerFichero (char *f, void *p, size_t cont) {
    struct stat s;
    ssize_t  n;
    int df,aux;

    if (stat (f,&s)==-1 || (df=open(f,O_RDONLY))==-1)
        return -1;
    if (cont==-1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
        cont=s.st_size;
    if ((n=read(df,p,cont))==-1){
        aux=errno;
        close(df);
        errno=aux;
        return -1;
    }
    close (df);
    return n;
}

ssize_t EscribirFichero (char *f, void *p, size_t cont,int overwrite) {
    ssize_t  n;
    int df,aux, flags=O_CREAT | O_EXCL | O_WRONLY;

    if (overwrite)
        flags=O_CREAT | O_WRONLY | O_TRUNC;

    if ((df=open(f,flags,0777))==-1)
        return -1;

    if ((n=write(df,p,cont))==-1){
        aux=errno;
        close(df);
        errno=aux;
        return -1;
    }
    close (df);
    return n;
}

void do_I_O_read (char*f , void* p, size_t cont){
    ssize_t n;
    if ((n=LeerFichero(f,p,cont))==-1)
        perror ("Imposible leer fichero");
    else
        printf ("leidos %lld bytes de %s en %p\n",(long long) n,f,p);
}

void do_I_O_write (char *f, void*p, size_t cont, int overwrite){
    ssize_t n;
    if ((n=EscribirFichero(f,p,cont, overwrite))==-1)
        perror ("Imposible escribir en el fichero");
    else
        printf ("escritos %lld bytes de %s en %p\n",(long long) n,f,p);
}

/* ******************************************************************************************************************* */
void recurseaux(int n){
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

    if (n>0)
        recurseaux(n-1);
}
/* ******************************************************************************************************************* */

void auxmemfill (void *p, size_t cont, unsigned char byte) {
    unsigned char *arr=(unsigned char *) p;
    size_t i;
    for (i=0; i<cont;i++)
        arr[i]=byte;

}
/* ******************************************************************************************************************* */
void auxmemdump(char *p, int tam){
    //char contenido[tam];
    char *contenido;
    contenido = p;
    for(int i = 0; i < tam; i++){
       printf("%c  ", contenido[i]);
    }
    printf("\n");
    for(int k =0; k< 3*tam; k++){
        printf("-");
    }
    printf("\n");
    for(int j = 0; j < tam; j++){
       printf("%d ", contenido[j]);
    }
    printf("\n");
}
/* ******************************************************************************************************************* */
void do_pmap () /*sin argumentos*/
{ pid_t pid;       /*hace el pmap (o equivalente) del proceso actual*/
    char elpid[32];
    char *argv[4]={"pmap",elpid,NULL};

    sprintf (elpid,"%d", (int) getpid());
    if ((pid=fork())==-1){
        perror ("Imposible crear proceso");
        puts("");
        return;
    }
    if (pid==0){
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute pmap (linux, solaris)");

        argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;
        if (execvp(argv[0],argv)==-1)/*No hay pmap, probamos procstat FreeBSD */
            perror("cannot execute procstat (FreeBSD)\n");

        argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;
        if (execvp(argv[0],argv)==-1)  /*probamos procmap OpenBSD*/
            perror("cannot execute procmap (OpenBSD)\n");

        argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
        if (execvp(argv[0],argv)==-1) /*probamos vmmap Mac-OS*/
            perror("cannot execute vmmap (Mac-OS)\n");
        exit(1);
    }
    waitpid (pid,NULL,0);
}

int getTam(int key, list l){
    pos i;
    mem_block *aux;
    int ret = -1;
    for(i = first(l); ret < 0 && !at_end(l,i); i = next(l,i)){
        aux = get(l,i);
        if(aux->key == key){
            ret = (int) aux->size;
        }
    }
    return ret;
}

/* ******************************************************************************************************************* */
/* ******************************************************************************************************************* */

int allocate(char *tokens[], list l){
    alloc_options allocOptions;
    emptyoptions(&allocOptions);
    int aux;
    int perm = 4;

    if(*tokens == NULL) {
        allocOptions.malloc =1;
        allocOptions.shared =1;
        allocOptions.mmap = 1;
        printMemList(l, allocOptions);
    }
    else if(strcmp(tokens[0], "-malloc") == 0){
        if(tokens[1] == NULL){
            allocOptions.malloc=1;
            printMemList(l, allocOptions);
        }
        else{
            aux = atoi(tokens[1]);
            if(aux > 0 ){
                void *a = malloc(aux);
                InsertElement(l,auxcreateMem_block(NULLKEY, a, aux, MALLOC, "malloc", NULLFD));
                printf("%d bytes assigned in the memory direction: %p\n", aux, a);
            }
            else {
                perror("Error: can't allocate less than 1 byte");
                puts("");
                return 0;
            }
        }
    }
    else if(strcmp(tokens[0], "-createshared") == 0){
        do_AllocateCreateshared(tokens[1],tokens[2], l);
        return 0;
    }
    else if(strcmp(tokens[0], "-shared") == 0){
        void *p;
        int cl;
        if(tokens[1] != NULL){
            cl = atoi(tokens[1]);
            p = ObtenerMemoriaShmget(cl, (size_t) 0,l);
            if(p != NULL){
                InsertElement(l, auxcreateMem_block(cl, p,(size_t) getTam(cl,l), SHARED, tokens[1], NULLFD));
                printf("Memoria compartida de clave %s en %p\n",tokens[1], p);
            }
        }
    }
    else if(strcmp(tokens[0], "-mmap") == 0){
        if(tokens[2] != NULL){
            perm = atoi(tokens[2]);
        }
        mapearFichero(tokens[1], perm, l);
        return 0;
    }
    else {
        printf("%s is not a valid option for allocate\n", tokens[0]);
    }
    return 0;
}
/* ******************************************************************************************************************* */

int deallocate(char *tokens[], list l){
    alloc_options allocOptions;
    emptyoptions(&allocOptions);
    mem_block *aux = get (l, first(l));
    pos i;

    if(*tokens == NULL){
        printMemList(l, allocOptions);
        return 0;
    }
    else if(strcmp(tokens[0], "-malloc")==0){
        int size;
        void *p;
        size = atoi(tokens[1]);
        if(size > 0) {
            for(i = first(l); aux->size != size && !at_end(l, i); i = next(l,i) ){
                aux = get(l,i);
            }
            if(aux->size == size){
                printf("Free %p size: %zu\n", aux->mem_addr, aux->size);
                p = cadtop(aux->mem_addr);
                freedata(i);
                deleteAtPosition(l,i,free);

                free(p);
                return 0;
            }
            else if(at_end(l,i)){
                perror("Element not found");
                puts("");
                return 0;
            }
            else{
                return 0;
            }
        }
        else{
            perror("Error:");
            puts("");
            return 0;
        }
    }
    else if(strcmp(tokens[0], "-shared")==0){
        void *p;
        if(tokens[1] == NULL){
            emptyoptions(&allocOptions);
            allocOptions.shared = 1;
            printMemList(l, allocOptions);
        }
        else{
            key_t  cl = atoi(tokens[1]);
            for(i = first(l);aux->key != cl && aux->type_alloc == SHARED && !at_end(l,i); i = next(l,i)){
                aux = get(l,i);
            }
            if(aux->key == cl){
                p = cadtop(aux->mem_addr);
                printf("Free %p key: %d\n", aux->mem_addr, aux->key);
                freedata(p);
                deleteAtPosition(l,i, free);
                return 0;
            }
            else if(at_end(l,i)){
                perror("Element not found");
                puts("");
                return 0;
            }
        }
            return 0;
    }
    else if(strcmp(tokens[0], "-mmap")==0){
        if(tokens[1] == NULL){
            emptyoptions(&allocOptions);
            allocOptions.mmap = 1;
            printMemList(l, allocOptions);
        }
        else{
            void* p;
            char* name = tokens[1];
            for(i = first(l); strcmp(aux->name,name)!=0 && aux->type_alloc == MMAP && !at_end(l,i); i = next(l,i)){
                aux = get(l,i);
            }
            if(strcmp(name, aux->name)){
                p = cadtop(aux->mem_addr);
                printf("Free %p name: %s\n", aux->mem_addr, aux->name);
                freedata(p);
                deleteAtPosition(l,i, free);
                free(p);
                return 0;
            }
            else if(at_end(l,i)){
                perror("Element not found");
                puts("");
                return 0;
            }
        }
        return 0;
    }
    else{
        //Borrar direccion de memoria de la lista
        void *p;

        for(i = first(l);strcmp(aux->mem_addr, tokens[0])!=0 && !at_end(l,i); i = next(l,i)){
            aux = get(l,i);
        }
        p = cadtop(tokens[0]);
        if(aux->mem_addr == p){
            printf("free %p\n", aux->mem_addr);
            deleteAtPosition(l,i, freedata);
            free(p);
        }
        return 0;
    }
}
/* ******************************************************************************************************************* */
int io (char* tokens[]){
    size_t tam = 25;
    char file[MAX_INPUT_SIZE];
    if(strcmp(tokens[0], "read")==0 && tokens[1] != NULL && tokens[2] != NULL && tokens[3] != NULL){
        strcpy(file, tokens[1]);
        void * p = cadtop(tokens[2]); /*convertimos de cadena a puntero*/
        if(tokens[3] != NULL)
        tam=(size_t) atoi(tokens[3]);
        do_I_O_read(file,p, tam);
    }
    else if(strcmp(tokens[0], "write")==0 && tokens[1] != NULL && tokens[2] != NULL && tokens[3] != NULL){
        int overwrite = 0;
        strcpy(file, tokens[1]);
        void * p = cadtop(tokens[2]); /*convertimos de cadena a puntero*/
        if(tokens[3] != NULL)
        tam=(size_t) atoi(tokens[3]);
        if(tokens[4] == NULL){
            overwrite = 0;
        }
        if(strcmp(tokens[4], "-o")==0){
            overwrite = 1;
        }
        do_I_O_write(file,p, tam, overwrite);
    }
    return 0;
}
/* ******************************************************************************************************************* */
int memory(char *tokens[], list l){
    alloc_options allocOptions;
    alloptions(&allocOptions);
    if(*tokens == NULL){
        printvariables();
        printfuncs();
        printMemList(l, allocOptions);
        return 0;
    }
    else if(strcmp(tokens[0], "-blocks")==0){
        printMemList(l,allocOptions);
        return 0;
    }
    else if(strcmp(tokens[0], "-funcs")==0){
        printfuncs();
        return 0;
    }
    else if(strcmp(tokens[0], "-vars")==0){
        printvariables();
        return 0;
    }
    else if(strcmp(tokens[0], "-all")==0){
        printvariables();
        printfuncs();
        return 0;
    }
    else if(strcmp(tokens[0], "-pmap")==0){
        do_pmap();
        return 0;
    }
    else{
        puts("Please, insert a valid prefix\n");
        return 0;
    }
}
/* ******************************************************************************************************************* */
int memdump(char* tokens[]){
    int tam = 25;
    void * p;
    if(*tokens == NULL){
        puts("Please, insert an address and a cont\n");
        return 0;
    }
    p= cadtop(tokens[0]);
    if(tokens[1] != NULL){
        tam = atoi(tokens[1]);
    }
    auxmemdump(p, tam);
    return 0;
}
/* ******************************************************************************************************************* */
int memfill(char *tokens[]){
    int tam = 128;
    char character = 'A';
    if(*tokens == NULL) {
        perror("Error");
        puts("");
        return 0;
    }
    void* p;
    p = cadtop(tokens[0]);
    if(tokens[1] != NULL){
        tam = atoi(tokens[1]);
    }
    if(tokens[2] != NULL){
        character = (tokens[2])[0];
    }
    auxmemfill(p, (size_t) tam, character);
    printf("Filling %p, %zu bytes with the char %c\n",p,(size_t) tam, character );
    return 0;
}
/* ******************************************************************************************************************* */
int recurse (char* tokens[]) {
    if(*tokens==NULL){
        perror("Error");
        puts("");
        return 0;
    }
    int n = atoi(tokens[0]);
    recurseaux(n);
    return 0;
}