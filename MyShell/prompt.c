#include "headers.h"
#include "prompt.h"


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

void print_myprompt(char* work_dir,char* username,char* sysname){
   memset(my_path, '\0', sizeof(my_path));
  int lwd = strlen(work_dir);
  int lrot = strlen(myroot);

    my_path[0]='~';

   if(work_dir == myroot){
     
      my_path[0]='~';

      }
  
   else if(lrot > lwd){
    strcat(my_path,work_dir);
   }
   else if(lwd > lrot && strstr(work_dir,myroot) !=NULL){
   strcat(my_path,work_dir + strlen(myroot));
   }
   else if (lwd > lrot){
   strcat(my_path,work_dir);

   }
   else if(lwd == lrot){

      if(strcmp(work_dir,myroot)!=0){
         strcat(my_path,work_dir);
      }
   }
  
   printf("<\033[1;33m%s\033[0m@\033[1;32m%s\033[0m:\033[1;34m%s\033[0m>", username, sysname, my_path);

 
}















