#include "headers.h"
#include "log.h"


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

void clear_my2dary() {
    for (int i = 0; i < ncmd; i++) {
        for (int j = 0; j < cmdlen; j++) {
            circular_2dary[i][j] = '\0';  // Clear each character
        }
    }
    index_log = 0;
    count_of_log = 0;
}


void write_to_file() {
    FILE* fl = fopen(filepath_cmd, "w");
    if (fl == NULL) {
        perror("Error opening file");
        return;
    }
    char state[cmdlen];
    for (int i = 0; i < count_of_log; i++) {
        int indx = (index_log - count_of_log + i + ncmd) % ncmd;
        if (indx < 0 || indx >= ncmd) {
            fprintf(stderr, "Index out of bounds: %d\n", indx);
            continue;
        }
        strcpy(state, circular_2dary[indx]);
        state[strcspn(state, "\n")] = '\0';

        fprintf(fl, "%s\n",state);
    }
    fclose(fl);
}
void delete_log_bysemi(char*cd){
  char * tt;
  char final[max]="";
  char delimit[] = ";";

  tt = strtok(cd,delimit);
  int x =0;
  while(tt != NULL){
    if(strstr(tt,"log") ==NULL){
       strcat(final,tt);

    }
    else{
      x=1;
    }
    tt = strtok(NULL,delimit);
    if(x!=1 && tt !=NULL){
      strcat(final,";");;
    }
    x=0;
  }
  strcpy(cd,final);

}

void delete_log_byamper(char*cd){
  char * tt;
  char final[max];
  char delimit[] = "&";

  tt = strtok(cd,delimit);
  while(tt != NULL){
    if(strstr(tt,"log") ==NULL){
       strcat(final,tt);
      //  strcat(final,";");

    }
    tt = strtok(NULL,delimit);
    if(tt != NULL){
      strcat(final,"&");;
    }
  }
  strcpy(cd,final);


}
void add_to_file_cmd(char *cmnd) {
    

    cmnd[strcspn(cmnd, "\n")] = '\0';
    if(strcmp(circular_2dary[index_log -1],cmnd)==0){
      return;
    }

    

    if(strstr(cmnd,"log")!= NULL){
       char dupcmd[max1];
       strcpy(dupcmd,cmnd);

       delete_log_bysemi(dupcmd);
      
       strcpy(cmnd,dupcmd);
    }
    if(cmnd[0] == '\0'){

      return;
    }
    strncpy(circular_2dary[index_log], cmnd, cmdlen - 1);
    circular_2dary[index_log][cmdlen - 1] = '\0';  // Ensure null-termination
    index_log = (index_log + 1) % ncmd;
    if (count_of_log < ncmd) {
        count_of_log++;
    }
    write_to_file();
}



void copy_filecmds_tomybuf(){
  FILE* fl =fopen(filepath_cmd,"r");
  if(fl == NULL)return;

  char line_cmd[cmdlen];
  while(fgets(line_cmd,sizeof(line_cmd),fl)!= NULL){
     line_cmd[strcspn(line_cmd, "\n")] = '\0';
     strncpy(circular_2dary[index_log],line_cmd,cmdlen);

     index_log = (index_log +1)%ncmd;
     if(count_of_log < ncmd){
      count_of_log ++;
     }

  }

  fclose(fl);
}

void create_file(){
  if(access(filepath_cmd,F_OK)==0){

    copy_filecmds_tomybuf();
    return;
  }
   FILE *file = fopen("commands.txt", "w"); // Create a new file or truncate if it exists
    if (file == NULL) {
        perror("Error creating file");
        return ; // Return a non-zero value to indicate failure
    }

    fclose(file);

}


void funlog(){
char print_cmmd[cmdlen];

      if (k==1){
        if(count_of_log < ncmd){
          for(int i=0;i<count_of_log;i++){
            strcpy(print_cmmd,circular_2dary[i]);
            printf("%s\n",print_cmmd);
          }
          
        }
        else{
          
            FILE *fle;
            char ch;

            // Open the file in read mode
            fle = fopen(filepath_cmd, "r");

            if (fle == NULL) {
                // Handle file not opening
                printf(RED"Could not open file."RESET"\n");
                // return 1;
            }

            while ((ch = fgetc(fle)) != EOF) {
                putchar(ch);
            }

            fclose(fle);

    


          }
        }
      
      else{
        
         mini_arg[1][strcspn(mini_arg[1], "\n")] = '\0';
        if(strcmp(mini_arg[1],"execute")==0){
          if (mini_arg[2] == NULL){
            printf(RED"INVALID ARGUMENT"RESET"\n");
            return;
          }
         
          int num = atoi(mini_arg[2]);
          if(num <=0 || num > 15){
            printf(RED"INVALID ARGUMENT"RESET"\n");
            return;
          }


          int mynum = (index_log - num + ncmd) % ncmd;
          char exe_cmd[max];
          if(*circular_2dary[mynum] == '\0'){
            printf(RED"Given number is invalid"RESET"\n");
            return;
          }
          strcpy(exe_cmd,circular_2dary[mynum]);
          tokenize_cmd(exe_cmd);
          run_commands(exe_cmd);
          return;
         
        }
        else if(strcmp(mini_arg[1],"purge")==0){
          FILE* ff = fopen(filepath_cmd,"w");
          if(ff == NULL){
            printf(RED"ERROR IN CLEARING THE FILE"RESET"\n");
            return;
          }
          fclose(ff);
          clear_my2dary();



        }
      }
      }























