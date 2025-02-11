#include "headers.h"
#include "bg.h"
#include "process.h"

void bgP(pid_t bgpid){

  int stat;
  MyProcess* MyProcess = get_myReqMyProcess(bgpid);
  if(MyProcess == NULL){
    printf(RED"Given pid is invalid"RESET"\n");
    return;
  }
  if(MyProcess->state != Stopped){
    printf(RED"MyProcess with given pid is not in stopped state"RESET"\n");
    return;

  }
  if(kill(bgpid,SIGCONT) < 0){
    perror(RED"Error in ssending signal : SIGCONT"RESET);
  }

  MyProcess->state = Running;

  updateStateMyProcess();
  return;

}










