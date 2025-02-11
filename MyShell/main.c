
#include "headers.h"


#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_WHITE "\033[0;37m"
#define COLOR_BLUE  "\033[0;34m"


char myroot[4096];
char*username;
char sysname[max];
char my_path[max];
char command[max1];
char **arry_commands;
int nc =0;  // no.of commands
int number = 0;
 char* req_path;
char working_dir[max];
char* prev_direc = NULL;
int k=0;
char ** mini_arg ;
char **back_miniarg;
char circular_2dary[ncmd][cmdlen];  
char foregrndProcess[max];


int index_log = 0;
int count_of_log = 0;

char filepath_cmd[cmdlen];
int global_var = 0;
char global_path[max];
double total_time_taken = 0.0;
char command_log[max] = "";

pid_t background_pids[max];
char background_names[max][max];
int bg_count = 0;  // Number of active background MyProcesses
void bg_fg(int bg,int j,char* CMMD,int saved_stdout,int file_dp);
int foregrnd_id = -1;
void ctrlD_cmd();



MyProcess MyProcessList[max22];

int MyProcessCount = 0;



int cmpPid(const void *a, const void *b) {
    MyProcess *MyProcessA = (MyProcess *)a;
    MyProcess *MyProcessB = (MyProcess *)b;

    return MyProcessA->pid - MyProcessB->pid;
}


void activities() {
  
    updateStateMyProcess();  
    
    
     qsort(MyProcessList, MyProcessCount, sizeof(MyProcess), cmpPid);
   
    for (int i = 0; i < MyProcessCount; i++) {
        printf("%d : %s - %s\n", MyProcessList[i].pid, MyProcessList[i].command,
               MyProcessList[i].state == Running ? "Running" : "Stopped");
    }
}
bool isOnlySpacesOrTabs(const char *str) {
    while (*str != '\0') {
        if (*str != ' ' && *str != '\t') {
            return false;
        }
        str++;
    }
    return true;
}


void delete_leading_spaces(char* sttr){
 
  int i=0;
  while(sttr[i] == ' '){
    i++;
  }
  int j=0;
  while(sttr[i] != '\0'){
    sttr[j++] = sttr[i++];
  }
  sttr[j] = '\0';

}
void remove_trailing_spaces(char *str) {
    int len = strlen(str);

    while (len > 0 && str[len - 1] == ' ') {
        len--;
    }

    str[len] = '\0';
}

void tokenize_cmd(char*command){
 

  char semi_cln[] = ";";
  char* token_arg;
  
  token_arg = strtok(command,semi_cln);
  int i=0;


  while(token_arg != NULL){
    delete_leading_spaces(token_arg);

    arry_commands[i] = token_arg;
    i++;
    token_arg = strtok(NULL,semi_cln);
  }

  nc = i;

}


void setNonBlockingState(struct termios *oldOne){
  struct termios newOne;
  tcgetattr(STDIN_FILENO,oldOne);
    newOne = *oldOne;
    newOne.c_lflag &= ~(ICANON | ECHO); 
    tcsetattr(STDIN_FILENO, TCSANOW, &newOne); 
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
  
}

void setNormalState(const struct termios *oldOne) {
    tcsetattr(STDIN_FILENO, TCSANOW, oldOne); 
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) & ~O_NONBLOCK); // Set blocking
}

int tryToGetRecent(){
  int mostRecentPid ;
  FILE *fp = fopen("/proc/loadavg","r");
  if(fp == NULL){
    perror("Failed to open given file");
    return -1;
  }

  char data[max];
  fgets(data,sizeof(data),fp);
  fclose(fp);

  char *getLastdata = strrchr(data,' ');
  if(getLastdata == NULL){
    return -1;

  }else{
    mostRecentPid = atoi(getLastdata+1);
    // return mostRecentPid;
  }

  return mostRecentPid;
}


void neonate_call(int reqSec){
  if(reqSec < 0){
    printf("\033[31mInvalid time argument\033[0m\n");
    return;
  }

  struct termios OldT;
  setNonBlockingState(&OldT);

  while(true){
    int RecProssId = tryToGetRecent();
    if(RecProssId == -1 || RecProssId <= 0){
      //error
      return;
    }else{
      printf("%d\n",RecProssId);
    }

    sleep(reqSec);
    char ch = getchar();
    if(ch == 'x'){
      break;
    }

  }
  setNormalState(&OldT);
  return;

}





void bg_fg(int bg,int j,char* CMMD, int saved_stdout,int file_dp){

  int bk ;
   char * mini_token;
  char* special = NULL;
  CMMD[strcspn(CMMD, "\n")] = '\0';
  if(isOnlySpacesOrTabs(CMMD)){
    return;
  }
  
  
   if(bg == 1){
          bk = 0;
          char back_command[max];
          if(j == -1){
            strcpy(back_command,CMMD);  
          }else{
            strcpy(back_command,arry_commands[j]);

          }
          
          char*start = back_command;
          char* pos_amper;
          while ((pos_amper = strchr(start, '&')) != NULL) {
          int partlen = pos_amper - start;
          char *temp_cmd = (char *)malloc((partlen + 1) * sizeof(char));
          if (temp_cmd == NULL) {
              perror("Memory allocation failed");
          }
          
          strncpy(temp_cmd, start, partlen);
          temp_cmd[partlen] = '\0';  // Null-terminate the string
          
          remove_trailing_spaces(temp_cmd);
          
          back_miniarg[bk] = (char *)malloc((strlen(temp_cmd) + 1) * sizeof(char));
          if (back_miniarg[bk] == NULL) {
              perror("\033[31mMemory allocation failed\033[0m");
              free(temp_cmd);  
          }
    
            strcpy(back_miniarg[bk], temp_cmd);
            
            
            bk++;
            free(temp_cmd);  // Free the temporary buffer once copied to back_miniarg
            
            start = pos_amper + 1;
           }
              start[strcspn(start, "\n")] = '\0';
              delete_leading_spaces(start);
              

              if(*start !='\0' ){
                
              if( !isspace(*start)){
                
                special = malloc(max);
                strcpy(special,start);
                // printf("spec:%s\n",special);
                

               }
              }
            if(bk == 0 && start != NULL){
              back_miniarg[bk] = (char *)malloc((strlen(start) + 1) * sizeof(char));
              if (back_miniarg[bk] == NULL) {
                  perror("Memory allocation failed");
                  free(start);  // Clean up allocated memory before exiting
              }
    
            strcpy(back_miniarg[bk], start);
            delete_leading_spaces(back_miniarg[bk]);
            remove_trailing_spaces(back_miniarg[bk]);
            bk++;

              
            }

            for(int i=0;i<bk;i++){
                char* minimini[max];
                int mm =0;
                char dl[] = " ";
                char* ttk;
                ttk = strtok(back_miniarg[i],dl);
                  while(ttk != NULL){
                      minimini[mm]=ttk;
                      mm++;
                      ttk = strtok(NULL,dl);
                    }
                  minimini[mm]=NULL;
                  for(int i=1;i<mm;i++){
                    minimini[i][strcspn(minimini[i], "\n")] = '\0';

                  }
                  int pi_id = fork();
                  if(pi_id < 0){
                    perror("fork failed");
                  }
                  if(pi_id == 0){
                    setpgid(0,0);
                    execvp(minimini[0],minimini);
                    printf("\033[31mERRONEOUS COMMAND -- BACKGROUND FAILED\033[0m\n");

                  }
                  else if(pi_id >0){
                    add_background_MyProcess(pi_id, minimini[0]); 
                    if(j==-1 && saved_stdout!= -1 && file_dp != -1){
                      dup2(saved_stdout,STDOUT_FILENO);
                       printf("[ %d ]\n",pi_id);
             
                       dup2(file_dp,STDOUT_FILENO);
                     

                    }else{
                       printf("[ %d ]\n",pi_id);

                    }
                     addMyProcess(pi_id, minimini[0], Running);
                  }

              }         
    }
    if(bg == 0 || special != NULL){   
    
        k =0;
        char command_copy[max];
        char space[] = " ";
        if(j == -1){
            strcpy(command_copy,CMMD);   
          }else{
        if(special != NULL ){
          remove_trailing_spaces(special);
          delete_leading_spaces(special);
          strcpy(command_copy,special);
          
        }
        
        else{
           strcpy(command_copy, arry_commands[j]);
        }
          }
        
        mini_token = strtok(command_copy,space);
          while(mini_token != NULL){             
                mini_arg[k] = mini_token;                  
                k++;
                mini_token = strtok(NULL,space);
              }
              mini_arg[k] = NULL;

              mini_arg[0][strcspn(mini_arg[0], "\n")] = '\0';
              // printf("cmd : %s.\n",mini_arg[0]);
 
              if(strcmp(mini_arg[0], "hop") == 0){

                funchop();
              }
              else if(strcmp(mini_arg[0],"reveal")==0){

                char stringg[max];
                strcpy(stringg,mini_arg[0]);
                for(int i=1;i<k;i++){
                  strcat(stringg," ");
                  strcat(stringg,mini_arg[i]);
                }
                Myreveal_cmd(stringg);

              }
              else if(strcmp(mini_arg[0],"log")==0){
                  funlog();
              }
              else if(strcmp(mini_arg[0],"proclore")==0){
                  int req_pid;
                  req_pid = getpid();
                  if(k==1){
                  funcproclore(req_pid);
                  }else{
                    req_pid = atoi(mini_arg[1]);
                    funcproclore(req_pid);
                  }
                  
              }
              else if(strcmp(mini_arg[0],"seek")==0){
    
                  run_seek_command(mini_arg, k);
              }
              else if(strcmp(mini_arg[0],"activities")==0){
                activities();
              }
              else if(strcmp(mini_arg[0] , "ping")==0){
                if(mini_arg[1]!= NULL){
                  pid_t ipid = atoi(mini_arg[1]);
                  if(mini_arg[2] != NULL){
                    int signal_num = atoi(mini_arg[2]);
                    ping(ipid,signal_num);
                  }else{
                      printf(RED"Incomplete command"RESET"\n");
                    }

                }else{
                  printf(RED"Incomplete command provided"RESET"\n");
                }
              }

              else if(strcmp(mini_arg[0],"fg")==0){
                if(mini_arg[1]!= NULL){
                  pid_t epid = atoi(mini_arg[1]);
                  fgP(epid);

                }else{
                       printf(RED"Incomplete command"RESET"\n");
                    }

              }

              else if(strcmp(mini_arg[0],"bg")==0){
                if(mini_arg[1]!= NULL){
                  pid_t bgpid = atoi(mini_arg[1]);
                  bgP(bgpid);

                }else{
                      printf(RED"Incomplete command"RESET"\n");
                    }

              }

              else if(strcmp(mini_arg[0],"neonate")==0){
                if(mini_arg[1]!= NULL){
                  if(strcmp(mini_arg[1],"-n")==0){
                    if(mini_arg[2]!=NULL){
                      int ReqSec = atoi(mini_arg[2]);
                      neonate_call(ReqSec);
                    }else{
                       printf(RED"Incomplete command"RESET"\n");
                    }
                  }
                }
              }
              else if(strcmp(mini_arg[0],"iMan")==0){
                if(mini_arg[1] != NULL){
                  iMan(mini_arg[1]);
                }
              }

              else{
                
                if(*mini_arg[0]!='\0'){
                  if(mini_arg[1]!=NULL){
                    mini_arg[1][strcspn(mini_arg[1], "\n")] = '\0';
                  }
                      int p_id = fork();
                    if(p_id == 0){
                       
                        execvp(mini_arg[0],mini_arg);
                        // perror("INVALID COMMAND ENTERED");
                        printf(RED"ERROR : '%s' is not a valid command "RESET"\n",mini_arg[0]);
                        return;
                        // continue;
                        ///return
                    }
                    else if(p_id > 0){ // parent
                      time_t begin_time = time(NULL);
                      int record_status;
                       foregrnd_id = p_id;
                        addMyProcess(p_id, mini_arg[0], Running);
                         while (waitpid(p_id, &record_status, WUNTRACED) > 0) {
                              if (WIFSTOPPED(record_status)) {
                                  
                                  break;
                              }
                          
                          }
                      if (WIFSTOPPED(record_status)) {
                      } else {
                          foregrnd_id = -1; // Clear foreground MyProcess ID if it was terminated
                      }
                      time_t finish_time = time(NULL);
                      double duration = finish_time - begin_time;
                      if(strcmp(mini_arg[0],"sleep")==0 && duration > 2){
                      total_time_taken = total_time_taken + duration;
                      
                        char command_time[max];
                          snprintf(command_time, sizeof(command_time), "%s : %.0f; ", mini_arg[0], duration);
                          strcat(command_log, command_time);
                         global_var =1;
                      }
                    }
                }

                }

     }

}


void run_commands(){

  mini_arg = (char**)malloc(max * sizeof(char*));
  back_miniarg = (char**)malloc(max * sizeof(char*));
  char* back_mintok;
  int bg =0;
  int bk ;
  char* special = NULL;
  char * mini_token;
  char space[] = " ";
  for(int j=0;j<nc;j++){
    // printf("hefbke\n");
   replace_aliases(arry_commands[j]);
    if (strstr(arry_commands[j],"&") != NULL){
          bg = 1;
    }
   
    if(strstr(arry_commands[j] , "|")!= NULL  ){
      execute_PipeLine(arry_commands[j]);
      // execute_redirection(arry_commands[j]);
      continue;
    }
    else if (strstr(arry_commands[j] , "<")!= NULL || strstr(arry_commands[j] , "<<")!= NULL ||strstr(arry_commands[j] , ">")!= NULL ){
      execute_redirection(arry_commands[j]);
      continue;

    }
    bg_fg(bg,j,arry_commands[j],-1,-1);
    bg = 0;
 
   
  }
}

void runshell(){

  while(1){
  handle_background_MyProcesses();
  if(global_var == 0){
    // printf("workk:%s.\n",working_dir);
  print_myprompt(working_dir,username,sysname);
  }
  else{
    getcwd(global_path,max);
    strcat(global_path,command_log);

    int x = total_time_taken;
    char str[50]; 
     sprintf(str, "%d", x);
    strcat(global_path,str);
    strcat(global_path,"s");
   memset(command_log, 0, sizeof(command_log));
  //  printf(".%s.\n",command_log);
  //  command_log[max] = NULL;
   total_time_taken = 0.0;
   print_myprompt(global_path,username,sysname);
   global_var = 0;

  }
  handle_background_MyProcesses();
  // fgets(command,max1,stdin);
  if (fgets(command, max1 ,stdin) == NULL) {
    // printf("khfbkff\n");
         ctrlD_cmd();
   
        }
        // printf("--%s.\n",command);
  if(command[0] == '\0' ||command[0] == '\n'){
    continue;
  }

 
  int sz = strlen(command);
  
  if(sz >= 2){
  if(command[sz-2]=='|'){
    printf(RED"ERROR IN THE GIVEN COMMAND"RESET"\n");
    continue;
  }

  }

  // replace_aliases(command);
  
  command[strcspn(command, "\n")] = '\0';

  char real_cmd[max] ="\0";
  strcpy(real_cmd,command);

  handle_background_MyProcesses();
  tokenize_cmd(command);

  run_commands();
  add_to_file_cmd(real_cmd);
  handle_background_MyProcesses();

  }

}

void ctrlD_cmd(){
  for (int i = 0; i < MyProcessCount; i++) {
        kill(MyProcessList[i].pid, SIGKILL);
  }
  MyProcessCount = 0;
  printf("\n");
  exit(0);
}

void signal_handler(int sig){
  if(sig == SIGINT){
    if(foregrnd_id != -1){
      kill(foregrnd_id,SIGINT);
       waitpid(foregrnd_id, NULL, 0);
      foregrnd_id = -1;
    }
  }
  if(sig == SIGTSTP){
    
    if(foregrnd_id != -1){
      MyProcess* p = get_myReqMyProcess(foregrnd_id);
      if(p==NULL){
        return;
      }
      p->state = Stopped;
      kill(foregrnd_id,SIGTSTP);

      foregrnd_id = -1;
    }
    return;

      
    }
}




 
int main(){
  
  getcwd(myroot,4096);
  strcpy(filepath_cmd,myroot);
  strcat(filepath_cmd,"/commands.txt");
  create_file();
  struct passwd *mydet = getpwuid(geteuid());
  username = malloc(4096 * (sizeof(char)));
  if(mydet != NULL){
    username = mydet->pw_name;
  }  
  gethostname(sysname,max);
   arry_commands = (char**)malloc(max * sizeof(char*));
  strcpy( working_dir , myroot);
 

  signal(SIGINT, signal_handler);   // Handle Ctrl-C (SIGINT)
  signal(SIGTSTP, signal_handler);  // Handle Ctrl-Z (SIGTSTP)
  
  execute_shrc();

  runshell();
  




}




















