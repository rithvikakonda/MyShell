#include "headers.h"
#include "sk.h"



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


void run_seek_command(char** mini_arg, int k) {
    bool search_files = true;
    bool search_dirs = true;
    bool exact_match = false;
    bool print_all = true;
    char* search_name = NULL;
    char* target_dir = ".";  
    bool found_single = false;
    bool is_file_found = false;
    bool is_dir_found = false;

    for (int i = 1; i < k; i++) {
        mini_arg[i][strcspn(mini_arg[i], "\n")] = '\0';
        if (strcmp(mini_arg[i], "-f") == 0) {
            search_dirs = false;  
            print_all = false;    
        } else if (strcmp(mini_arg[i], "-d") == 0) {
            search_files = false;  // Only search for directories
            print_all = false;    
        } else if (strcmp(mini_arg[i], "-e") == 0) {
            exact_match = true;  // Exact name match required
            print_all = false;   
        } else if (search_name == NULL) {
            search_name = mini_arg[i]; 
        } else {
            target_dir = mini_arg[i];  
        }
    }

    if (search_name == NULL) {
        fprintf(stderr, "No search name provided!\n");
        return;
    }

    if (!search_dirs && !search_files) {
        fprintf(stderr, "Invalid flags!\n");
        return;
    }

    search_directory(target_dir, search_name, search_files, search_dirs, exact_match, &found_single, 
    &is_file_found, &is_dir_found);

     if (!found_single && exact_match) {
        // printf("heelo\n");
        printf("No match found!\n");
    }

}
void search_directory(const char *base_path, const char *search_name, bool search_files,
 bool search_dirs, bool exact_match, bool *found_single, bool *is_file_found, bool *is_dir_found) {
    DIR *dir;
    struct dirent *entry;
    char path[max];
    struct stat statbuf;
    bool match_found = false;
  
    if (!(dir = opendir(base_path))) {
        printf("failed to open directory\n");
        // perror("opendir");

        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", base_path, entry->d_name);

        if (stat(path, &statbuf) == -1) {
            
            continue;
        }

        int is_dir = S_ISDIR(statbuf.st_mode);
        int is_file = S_ISREG(statbuf.st_mode);

        if ((search_dirs && is_dir) || (search_files && is_file) || !search_files && !search_dirs) {
            if (exact_match && strcmp(entry->d_name, search_name) == 0) {
                if (is_file) {
                    *is_file_found = true;
                    if (*found_single) {
                       
                        printf("No match found!\n");
                        closedir(dir);
                        return;
                    }
                    FILE *file = fopen(path, "r");
                    if (file) {
                        int c;
                        while ((c = fgetc(file)) != EOF) {
                            putchar(c);
                        }
                        fclose(file);
                        printf("\n");
                        *found_single = true;
                    } else {
                        fprintf(stderr, "Missing permissions for task!\n");
                    }
                } else if (is_dir) {
                    *is_dir_found = true;
                    if (*found_single) {
                       
                        printf("No match found!\n");
                        closedir(dir);
                        return;
                    }
                    if (chdir(path) == 0) {  
                        char rel_path[max];
                        getcwd(rel_path,max);
                        strcpy(working_dir,rel_path);
                        *found_single = true;
                    } else {
                        fprintf(stderr, "Missing permissions for task!\n");
                    }
                }
                match_found = true;
                *found_single = true;
            } else if (!exact_match && strstr(entry->d_name, search_name) != NULL) {
                if (is_file) {
                    printf("\033[32m%s\033[0m\n", path);
                } else if (is_dir) {
                    printf("\033[34m%s\033[0m\n", path);
                }
                match_found = true;
            }
        }

        if (is_dir) {
            search_directory(path, search_name, search_files, search_dirs, exact_match, found_single, is_file_found, is_dir_found);
        }
    }

    closedir(dir);

    // if (!match_found && !exact_match) {
    //     printf("No matches found!\n");
    // }
}









