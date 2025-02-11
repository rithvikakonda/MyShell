#include "headers.h"
#include "hop.h"


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

void funchop(){

   char resolved_path[max];
  if(k == 1 ){
            
           req_path = myroot;
           strcpy(working_dir,myroot);
            chdir(myroot);
            printf("%s\n",req_path);

          }
          
          for(int a=1;a<k;a++){
            
             mini_arg[a][strcspn(mini_arg[a], "\n")] = '\0';

            if(strcmp(mini_arg[a],"~")==0 ){
              
               req_path = myroot;
              
               strcpy(working_dir,myroot);
               chdir(myroot);
             
               printf("%s\n",req_path);

             
            }
             
            else if (mini_arg[a][0] == '~') {
                  if (mini_arg[a][1] == '/' ) {
                      // Handle ~/ or ~ alone
                      char g_path[max];
                      strcpy(g_path,myroot);
                      strcat(g_path,"/");
                      if(mini_arg[a][2]!='\0'){

                      snprintf(resolved_path, sizeof(resolved_path), "%s%s", g_path ,&mini_arg[a][2]);
                      }

                  } 
                 
                if (access(resolved_path,F_OK)==0){
                    if(chdir(resolved_path)==0){
                      if (prev_direc != NULL) {
                        free(prev_direc);  // Free previous directory if already set
                          }
                      prev_direc = strdup(working_dir);  // Save current directory as previous
                      getcwd(working_dir,max);
                      printf("%s\n",working_dir);
                    }
                else {
                  printf(RED"UNABLE TO CHANGE"RESET"\n");
                 }
              
            }
            }

            else if(strcmp(mini_arg[a],"..")==0){
              
              if(chdir("..")==0){
                if (prev_direc != NULL) {
                                free(prev_direc);  // Free previous directory if already set
                            }
                prev_direc = strdup(working_dir); 
                getcwd(working_dir,max);
                printf("%s\n",working_dir);
              }else{
                printf(RED"UNABLE TO CHANGE"RESET"\n");
              }    


            }
            else if(strcmp(mini_arg[a],".")==0){
              char printdir[max];

              getcwd(printdir,max);
              

            printf("%s\n",printdir); 
           }
            else if(strcmp(mini_arg[a],"-")==0){
              if(prev_direc == NULL){
                printf("PWD not found\n");

              }else{
                printf("%s\n", prev_direc);
                if (chdir(prev_direc) == 0) {
                   char *temp = strdup(working_dir);  
                   strcpy(working_dir, prev_direc);
                   free(prev_direc);  
                    prev_direc = temp; 
               } else {
                     printf(RED"UNABLE TO CHANGE"RESET"\n");
                     }
                
              }
             
            }
             else if (mini_arg[a][0] == '/') {
                        
                        if (chdir(mini_arg[a]) == 0) {
                            if (prev_direc != NULL) {
                                free(prev_direc); 
                            }
                            prev_direc = strdup(working_dir);  
                            getcwd(working_dir, max); 
                            printf("%s\n", working_dir);
                        } else {
                           printf(RED"UNABLE TO CHANGE"RESET"\n");
                        }
             }
            else if (access(mini_arg[a],F_OK)==0){
                 if(chdir(mini_arg[a])==0){
                   if (prev_direc != NULL) {
                     free(prev_direc);  
                      }
                  prev_direc = strdup(working_dir); 
                  getcwd(working_dir,max);
                  printf("%s\n",working_dir);
                 }
                 else {
                 printf(RED"UNABLE TO CHANGE"RESET"\n");
                 }

            }else{
              printf(RED"invalid argument"RESET"\n");

            }
           
            
          }

        
}
















    
