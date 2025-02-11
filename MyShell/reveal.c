#include "headers.h"
#include "reveal.h"


extern char myroot[4096];
extern char*username;
extern char sysname[max];
extern char my_path[max];
extern char command[max1];
extern char **arry_commands;
extern int nc ;  // no.of commands
extern int number;
extern  char* req_path;
extern char working_dir[max];
extern char* prev_direc ;
extern int k;
extern char ** mini_arg ;
extern char circular_2dary[ncmd][cmdlen];  

extern int index_log ;
extern int count_of_log;

extern char filepath_cmd[cmdlen];



void Myreveal_cmd(char *input) {
    Reveal_Struct options = parse_Myreveal_cmd(input);
    char *myreslov_path = resolve_path(options.path);
    if(myreslov_path == NULL){
        printf(RED"Given file dont exist"RESET"\n");
        return;
    }

    int entry_count = 0;
    struct dirent **MyEntries = Direc_read(myreslov_path, options.show_all, &entry_count);
    if(MyEntries == NULL){
        return;
    }

    sort_MyEntries_lexicographically(MyEntries, entry_count);
    display_MyEntries(MyEntries, entry_count, options.show_details, myreslov_path);

    delete_freeMyEntries(MyEntries, entry_count);
    free(myreslov_path);
}

Reveal_Struct parse_Myreveal_cmd(char *input) {
    Reveal_Struct options = {false, false, "."};  // Default to current directory
    char newinput[max];
   
    strncpy(newinput,input+7 ,strlen(input)-7+1);
    
    char *token = strtok(newinput, " ");
    while (token != NULL) {
    
      token[strcspn(token, "\n")] = '\0';
        if (token[0] == '-' && strlen(token)!=1) {
            for (int i = 1; token[i] != '\0'; i++) {
                if (token[i] == 'a') options.show_all = true;
                if (token[i] == 'l') options.show_details = true;
            }
        } else {
            options.path = token;  // The path is found after flags
        }
        token = strtok(NULL, " ");
    }

    return options;
}

char* resolve_path(char *input_path) {
    if(input_path == NULL || input_path[0] == '\0'){
      input_path[0] = '.';
    }
    char *myreslov_path = malloc(max);  // Use max for maximum path size
    if (!myreslov_path) {
        perror("malloc");
        return NULL;
    }
    
     if (input_path[0] == '~') {
        
        char* homepath = malloc(max);
        // strcpy()
        strcpy(homepath,"/home/");
        strcat(homepath,username);
        strcpy(myreslov_path,homepath);
        
    }
      else if (strcmp(input_path, "..") == 0) {
            if (!getcwd(myreslov_path,max)) {
                perror("getcwd");
                free(myreslov_path);
                return NULL;
            }
            char *last_slash = strrchr(myreslov_path, '/');
            if (last_slash != NULL && last_slash != myreslov_path) {
                *last_slash = '\0';
            } else {
                strcpy(myreslov_path, "/");
            }
        }
   
     else if (input_path[0] == '-') {  
        if(prev_direc != NULL){
          strcpy(myreslov_path,prev_direc);
        }
        else {
          printf("error prev directory dont exist\n");
          return NULL;
        }
     } 
    else if (input_path[0] == '.') {
        if (!getcwd(myreslov_path, max)) {
            perror("getcwd");
            free(myreslov_path);
            return NULL;
        }
    }
    else {
        int lln = strlen(input_path);
        strncpy(myreslov_path, input_path, lln);
        myreslov_path[lln] = '\0';  // Ensure null termination
        if(access(myreslov_path,F_OK)!=0){
            return NULL;
        }

    }

    myreslov_path[strcspn(myreslov_path, "\n")] = '\0';
    return myreslov_path;
}


int alphasort(const struct dirent **a, const struct dirent **b) {
    return strcasecmp((*a)->d_name, (*b)->d_name);
}

int filter_hidden(const struct dirent *entry) {
    return (entry->d_name[0] != '.');
}

struct dirent** Direc_read(char *myreslov_path, bool show_all, int *entry_count) {
    if (myreslov_path == NULL) {
        fprintf(stderr, "ERROR: Invalid path\n");
        return NULL;
    }

    struct dirent **MyEntries = NULL;
    if (show_all) {
        *entry_count = scandir(myreslov_path, &MyEntries, NULL, alphasort);
    } else {
        *entry_count = scandir(myreslov_path, &MyEntries, filter_hidden, alphasort);
    }

    if (*entry_count < 0) {
        perror("scandir");
        return NULL;
    }

    return MyEntries;
}
int compare_MyEntries(const void *a, const void *b) {
    struct dirent *entryA = *(struct dirent **)a;
    struct dirent *entryB = *(struct dirent **)b;
    return strcmp(entryA->d_name, entryB->d_name);
}

void sort_MyEntries_lexicographically(struct dirent **MyEntries, int count) {
    qsort(MyEntries, count, sizeof(struct dirent*), compare_MyEntries);
}


void display_MyEntries(struct dirent **MyEntries, int count, bool show_details, char *myreslov_path) {
    struct stat file_stat;
    char full_path[max];
    long tt_blcks=0;
    for (int i = 0; i < count; i++) {
        snprintf(full_path, max, "%s/%s", myreslov_path, MyEntries[i]->d_name);
        if (stat(full_path, &file_stat) == 0) {
           tt_blcks += file_stat.st_blocks; // Accumulate blocks
        } else {
            perror("stat");
        }
    }

    if (show_details) {
       
        printf("total %ld\n", tt_blcks / 2); // Divide by 2 to convert to 1K blocks
    }

    for (int i = 0; i < count; i++) {
        snprintf(full_path, max, "%s/%s", myreslov_path, MyEntries[i]->d_name);
        if (stat(full_path, &file_stat) == 0) {

            if (show_details) {
                Display_Details(file_stat, MyEntries[i]->d_name);
            } else {
            
                if (S_ISDIR(file_stat.st_mode)) {
                    printf(COLOR_BLUE "%s" COLOR_RESET "\n", MyEntries[i]->d_name);
                } else if (file_stat.st_mode & S_IXUSR) {
                    printf(COLOR_GREEN "%s" COLOR_RESET "\n", MyEntries[i]->d_name);
                } else {
                    printf(COLOR_WHITE "%s" COLOR_RESET "\n", MyEntries[i]->d_name);
                }
            }
        } else {
            perror("stat");
        }
    }
}
void Display_Details(struct stat file_stat, const char *name) {
    char time_buf[80];
    struct tm *timeinfo = localtime(&file_stat.st_mtime);
    strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", timeinfo);

    char prmsns[11];
    Permsns_Cnvt(file_stat.st_mode, prmsns);

   const char *color;
    if (S_ISDIR(file_stat.st_mode)) {
        color = COLOR_BLUE; 
    } else if (file_stat.st_mode & S_IXUSR) {
        color = COLOR_GREEN; 
    } else {
        color = COLOR_WHITE;  
    }

    printf("%s %3ld %-8s %-8s %8ld %s %s%s%s\n",
           prmsns,
           file_stat.st_nlink,
           getpwuid(file_stat.st_uid)->pw_name,
           getgrgid(file_stat.st_gid)->gr_name,
           file_stat.st_size,
           time_buf,
           color, 
           name,
           COLOR_RESET);
}

void Permsns_Cnvt(mode_t mode, char *prmsns) {
    strcpy(prmsns, "----------");

   
    if (S_ISDIR(mode)) prmsns[0] = 'd';
    else if (S_ISLNK(mode)) prmsns[0] = 'l';
    else if (S_ISCHR(mode)) prmsns[0] = 'c';
    else if (S_ISBLK(mode)) prmsns[0] = 'b';
    else if (S_ISFIFO(mode)) prmsns[0] = 'p';
    else if (S_ISSOCK(mode)) prmsns[0] = 's';

   
    if (mode & S_IRUSR) prmsns[1] = 'r';
    if (mode & S_IWUSR) prmsns[2] = 'w';
    if (mode & S_IXUSR) prmsns[3] = 'x';


    if (mode & S_IRGRP) prmsns[4] = 'r';
    if (mode & S_IWGRP) prmsns[5] = 'w';
    if (mode & S_IXGRP) prmsns[6] = 'x';

    if (mode & S_IROTH) prmsns[7] = 'r';
    if (mode & S_IWOTH) prmsns[8] = 'w';
    if (mode & S_IXOTH) prmsns[9] = 'x';
}

void delete_freeMyEntries(struct dirent **MyEntries, int count) {
    free(MyEntries);  
}















