#include "headers.h"
#include "ping.h"



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
extern char **back_miniarg;
extern char circular_2dary[ncmd][cmdlen];  

extern int index_log ;
extern int count_of_log;

extern char filepath_cmd[cmdlen];
extern int global_var ;
extern char global_path[max];
extern double total_time_taken ;
extern char command_log[max];
extern pid_t background_pids[];
extern char background_names[max][max];
extern int bg_count ;  
extern int foregrnd_id ;
extern MyProcess MyProcessList[max22];
extern int MyProcessCount;



void ping(pid_t pid,int signum){

  signum = signum % 32;
  MyProcess* ReqMyProcess = get_myReqMyProcess(pid);
  if(ReqMyProcess == NULL){
    printf(RED"Process with given pid is not present"RESET"\n");
    return;
  }
  if(kill(pid,signum)==-1){
    if (errno == ESRCH) { // error no such MyProcess
            printf(RED"No such MyProcess found"RESET"\n");
        } else {
            perror(RED"Error sending signal"RESET);
        }
        return;

  }
  printf("Sent signal %d to MyProcess with pid %d\n",signum,pid);

  if(signum == SIGKILL || signum == SIGTERM){
    for(int i=0;i<MyProcessCount;i++){
      if(MyProcessList[i].pid == pid){
        removeMyProcessFromList(i);

      }
    }
    
  }
  else if(signum == SIGSTOP || signum == SIGTSTP || signum == SIGTTIN || signum == SIGTTOU){
    ReqMyProcess->state = Stopped;
  }

}
















