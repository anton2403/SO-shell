/*  
Carlos Martinez Rabunal , carlos.martinez4@udc.es
Anton Lopez Nunez , anton.lopez.nunez@udc.es
*/

#include "p1.h"

/* FUNCIONES AUXILIARES ********************************************************************************************** */
int isADir(struct stat stat){
     if(stat.st_mode & S_IFDIR ){
         return 1;
     }
     else if(stat.st_mode & S_IFREG ){
         return 0;
     }
     else{
         return -1;
     }
 }

unsigned char fileType(char *fileName){
    struct stat sdata;

    if(stat(fileName, &sdata)==-1){
        return DT_UNKNOWN;
    }
    switch (sdata.st_mode & S_IFMT)
    {
        case S_IFBLK:  return DT_BLK;
        case S_IFCHR:  return DT_CHR;
        case S_IFDIR:  return DT_DIR;
        case S_IFIFO:  return DT_FIFO;
        case S_IFLNK:  return DT_LNK;
        case S_IFREG:  return DT_REG;
        case S_IFSOCK: return DT_SOCK;
        default:       return DT_UNKNOWN;
    }
}

long int findSize(char* name) {
    struct stat stat1;

    if(lstat(name, &stat1)==-1){
        perror("Error: ");
        return -1;
    }
    else{
        return stat1.st_size;
    }
}

unsigned int convertToOctal(unsigned int decimal){
    int i = 1;
    unsigned int octal =0;
    while(decimal != 0){
        octal += ((int) decimal % 8) * i;
        decimal /= 8;
        i *= 10;
    }
    return (unsigned int)octal;
}

char *switch_mode (unsigned int num){
    switch (num){
        case 0:
            return "---";
        case 1:
            return "--x";
        case 2:
            return "-w-";
        case 3:
            return "-wx";
        case 4:
            return "r--";
        case 5:
            return "r-x";
        case 6:
            return "rw-";
        case 7:
            return "rwx";
        default:
            return "";
    }
}

void format_mode(struct stat stat, char* solution){
    unsigned int owner, group, other;
    char aux[MAX_INPUT_SIZE];
    uintmax_t mode = stat.st_mode;

    if(isADir(stat)){
        strcpy(aux, "d");
    }
    else if(isADir(stat) == 0){
        strcpy(aux,"f");
    }
    else{
        strcpy(aux, "other");
    }

    mode = convertToOctal(mode);

    other = mode%10;
    group = (mode/10) %10;
    owner = (mode/100) %10;

    strcat(aux, switch_mode(owner));
    strcat(aux, switch_mode(group));
    strcat(aux, switch_mode(other));

    strcpy(solution, aux);
}

void process_file(char *f){
    if(findSize(f) >= 0){
        printf ("%-10ld  %10s\n", findSize(f), f);
    }
    else {
        /* Si ha pasado algo, sólo decimos el nombre */
        printf("(No info.) %10s\n", f);
    }
}
/* ******************************************************************************************************************* */

int statItem(char *item, struct stat_options statOptions) {
    char perm[MAX_INPUT_SIZE];
    struct stat file_stat;
    struct tm time;
    time_t t;
    char *nullTokens[MAX_TOKENS];

    if(item == NULL) {
        carpeta(nullTokens);
        return 0;
    }
    else {
        if (lstat(item, &file_stat) == -1) {
            perror("Error: ");
            puts("");
            return 0;
        } else {
            t = file_stat.st_mtim.tv_sec; // Convert the given time to the computer's local time zone for correct formatting.
            time = *localtime(&t);

            struct group *grp = getgrgid(file_stat.st_gid);
            struct passwd *user = getpwuid(file_stat.st_uid);

            format_mode(file_stat, perm);

            if(statOptions.lon == 0 && statOptions.link ==0 && statOptions.acc == 0){
                printf("%s\t%ld\n", item, file_stat.st_size);
            } else if (statOptions.link == 1) {
                printf("%s Links: %lu\n", item, file_stat.st_nlink);
            } else if (statOptions.acc == 1) {
                printf("Last access to %s: %04d/%02d/%02d-%02d:%02d\n", item,
                       time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min);
            } else if (statOptions.lon == 1) {
                printf("%04d/%02d/%02d-%02d:%02d\t", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday,
                       time.tm_hour,
                       time.tm_min);
                printf("links: %lu\t", file_stat.st_nlink);
                printf("inode: %lu\t", file_stat.st_ino);
                printf("Owner: %s\t", grp->gr_name);
                printf("Mode: %s %jo (octal)     %s\t", user->pw_name, (uintmax_t) file_stat.st_mode, perm);
                printf("Size: %ld\t", file_stat.st_size);
                printf("Name: %s\n", item);
            }
            else{

            }
        }
    }
    return 0;
}

void list_dir(char* path, struct list_options listOptions){
    char direction[MAX_INPUT_SIZE];
    char actual[MAX_INPUT_SIZE];

    getcwd(direction, MAX_INPUT_SIZE);
    strcpy(actual,direction);
    strcat(direction, "/");
    strcat(direction, path);
    DIR *D = opendir(direction);
    if (D == NULL) {
        printf("can't open %s", path);
        perror("Error");
        puts("");
    }

    if((listOptions.statOptions.acc ==0 && listOptions.statOptions.link ==0 && listOptions.statOptions.lon == 0
        && listOptions.reca ==0 && listOptions.recb ==0 && listOptions.hid ==0) && chdir(path) ==0){
        //simple list
        printf("********** %s\n", path);
        struct dirent *ENT;
        chdir(path);
        while ((ENT = readdir(D)) != NULL) {
            if (ENT->d_name[0] != '.') {
                process_file(ENT->d_name);
            }
        }
        closedir(D);
        chdir(actual);
    }
    else if ((listOptions.statOptions.acc ==1 || listOptions.statOptions.link ==1 || listOptions.statOptions.lon == 1) && chdir(path) == 0) {
        //List with stat options
        printf("********** %s\n",path);
        struct dirent *ENT;
        while ((ENT = readdir(D)) != NULL) {
            if ((strcmp(ENT->d_name, ".") != 0) && (strcmp(ENT->d_name, "..") != 0)) {
                statItem(ENT->d_name,listOptions.statOptions);
            }
        }
        closedir(D);
        chdir(actual);
    }

    else if(listOptions.hid == 1 && chdir(path) ==0){
        printf("********** %s\n", path);
        struct dirent *ENT;
        while ((ENT = readdir(D)) != NULL) {
            if ((strcmp(ENT->d_name, ".") != 0) && (strcmp(ENT->d_name, "..") != 0)) {
                process_file(ENT->d_name);
            }
            else if(strcmp(ENT->d_name, ".") == 0){
                printf("%-10ld %10s\n", findSize("."), ".");
                chdir(path);
            }
            else if(strcmp(ENT->d_name, "..") == 0){
                printf("%-10ld %10s\n", findSize(".."), "..");
                chdir(direction);
                chdir(path);
            }
        }
        closedir(D);
        chdir(actual);
    }

    else {
        printf("cant open %s", path);
    }

}

void recursivelistA(char* path, struct list_options listOptions) {
    char cwd[MAX_PATH];
    unsigned char type;
    DIR *dir;

    getcwd(cwd, MAX_PATH);
    strcat(cwd, "/");
    strcat(cwd, path);
    struct dirent * dirent;

    type= fileType(path);
    if(type==DT_DIR){
        dir= opendir(cwd);
        if(dir==NULL) perror("Error: ");
        else{
            listOptions.reca=0;
            list_dir(path, listOptions);
            chdir(cwd);
            for(dirent= readdir(dir);dirent!=NULL; dirent= readdir(dir)){
                type= fileType(dirent->d_name);
                if(type==DT_DIR && dirent->d_name[0]!='.'){
                    chdir(path);
                    recursivelistA(dirent->d_name, listOptions);
                    chdir("..");
                }
            }
        }
    }
    else{
        list_dir(path, listOptions);
    }
}

void recursivelistB(char* path, struct list_options listOptions) {
    char cwd[MAX_PATH];
    char aux[MAX_PATH];
    unsigned char type;
    DIR *dir;

    getcwd(cwd, MAX_PATH);
    strcpy(aux, cwd);
    strcat(cwd, "/");
    strcat(cwd, path);
    struct dirent * dirent;

    type= fileType(path);
    if(type==DT_DIR){
        dir= opendir(cwd);
        if(dir==NULL) perror("Error: ");
        else{
            chdir(cwd);
            for(dirent= readdir(dir);dirent!=NULL; dirent= readdir(dir)){
                type= fileType(dirent->d_name);
                if(type==DT_DIR && dirent->d_name[0]!='.'){
                    recursivelistB(dirent->d_name, listOptions);
                }
            }
            chdir(aux);
            closedir(dir);
            listOptions.recb=0;
            list_dir(path, listOptions);
            listOptions.recb=0;

        }
    }
    else{
        list_dir(path, listOptions);
    }
}

void delete_item(char *path) {
    struct stat st;
    if(lstat(path, &st) == -1) {
        printf("could not open %s:\n", path);
        perror("Error");
        puts("");
    }

    if(isADir(st)) { // Directorio
        DIR *d = opendir(path);
        struct dirent *ent;

        if(d == NULL) {
            printf("could not open %s:\n", path);
            perror("Error");
            puts("");
        }

        while((ent = readdir(d)) != NULL) {
            char new_path[MAX_PATH];

            if(strcmp(ent->d_name, ".") ==0 || strcmp(ent->d_name, "..")==0)
                continue;

            sprintf(new_path, "%s/%s", path, ent->d_name);
            delete_item(new_path);
        }
        closedir(d);
    }

    if(remove(path) == -1) {
        printf("Could not delete %s:\n", path);
        perror("Error");
        puts("");
    }
    printf("Deleting  %s ...\n", path);
}

/* ******************************************************************************************************************* */

int create(char *tokens[]){
    if(*tokens == NULL){
        carpeta(tokens);
        return 0;
    }
    else{
        if(strcmp (tokens[0], "-f") == 0 && tokens[1] != NULL){
            open(tokens[1], O_CREAT, (S_IRUSR | S_IWUSR | S_IXUSR));
            printf("%s file has been created\n", tokens[1]);
            return 0;
        }
        if(tokens[0] != NULL && mkdir(tokens[0], S_IRUSR | S_IWUSR | S_IXUSR) == 0){
            printf("%s directory has been created\n", tokens[0]);
            return 0;
        }
        else{
            perror("Error:");
            puts("");
            return 0;
        }
    }
}

int stat_fun(char *tokens[]){
    struct stat_options statOptions;
    char item[MAX_INPUT_SIZE];

    statOptions.link = 0;
    statOptions.lon = 0;
    statOptions.acc = 0;

    if (strcmp(tokens[0], "-acc") == 0) {
        statOptions.acc = 1;
    } else if (strcmp(tokens[0], "-link") == 0) {
        statOptions.link = 1;
    } else if (strcmp(tokens[0], "-long") == 0) {
        statOptions.lon = 1;
    }


    if(statOptions.lon == 0 && statOptions.link == 0 && statOptions.acc == 0) {
        for(int i =0; tokens[i] != NULL; i++){
            strcpy(item, tokens[i]),
                    statItem(item, statOptions);
        }

    } else {
        for(int i =1; tokens[i] != NULL; i++){
            strcpy(item, tokens[i]),
                    statItem(item, statOptions);
        }
    }
    return 0;
}

int list_fun (char *tokens[]) {
    char path[MAX_INPUT_SIZE];

    struct list_options listOptions;

    listOptions.statOptions.link = 0;
    listOptions.statOptions.acc = 0;
    listOptions.statOptions.lon = 0;
    listOptions.hid = 0;
    listOptions.reca = 0;
    listOptions.recb = 0;

    if (*tokens == NULL) {
        carpeta(tokens);
        return 0;
    }
    else if (tokens[0] != NULL) {
        if (strcmp(tokens[0], "-long") == 0) {
            listOptions.statOptions.lon = 1;
        } else if (strcmp(tokens[0], "-acc") == 0) {
            listOptions.statOptions.acc = 1;
        } else if (strcmp(tokens[0], "-link") == 0) {
            listOptions.statOptions.link = 1;
        } else if (strcmp(tokens[0], "-hid") == 0) {
            listOptions.hid = 1;
        } else if (strcmp(tokens[0], "-reca") == 0) {
            listOptions.reca = 1;
        } else if (strcmp(tokens[0], "-recb") == 0) {
            listOptions.recb = 1;
        } else {//List [dir]
            for(int i =0; tokens[i] != NULL; i++){
                //strcpy(path, tokens[i]);
                list_dir(tokens[i], listOptions);
            }
            return 0;
        }
    }
    if(listOptions.statOptions.lon==1 || listOptions.statOptions.acc ==1|| listOptions.statOptions.link == 1 || listOptions.hid ==1 ) {
        for (int i = 1; tokens[i] != NULL; i++) {
            strcpy(path, tokens[1]);
            list_dir(path, listOptions);
        }
        return 0;
    }
    if(listOptions.reca ==1){
        strcpy(path, tokens[1]);
        recursivelistA(path, listOptions);
        chdir("..");
        return 0;
    }
    else if(listOptions.recb ==1){
        strcpy(path, tokens[1]);
        recursivelistB(path, listOptions);
        return 0;
    }
    else{
        return 0;
    }
}

int delete(char* tokens[]) {
    int i = 0;
    int maxi = 0;
    while(tokens[maxi] != NULL){
        maxi = maxi +1;
    }
    do {
        /*DIR * dir = opendir(tokens[i]);
        struct dirent *dirent;
        int content = 0;

        if(dir == NULL){
            perror("Error:");
            puts("");
            return 0;
        }*/
        /*else {
            while((dirent = readdir(dir)) != NULL){
                if(content > 2){
                    puts("The directory is not empty");
                }
                content = content +1;
            }
        } if(content <= 2){*/
        if(remove(tokens[i]) == 0){
            puts("Succesful delete");
        } else{
            perror("Error:");
            puts("");
        }
        /*}*/
        i = i+1;
    }
    while(i < maxi);
    return 0;
}

int deltree(char *tokens[] ) {
    for(int i=0; tokens[i] != NULL; i++) {
        delete_item(tokens[i]);
    }
    return 0;
}