#ifndef MyProcess_H
#define MyProcess_H
#include "headers.h"

enum Sp { 
   Running, 
   Stopped
} ;
typedef enum Sp MyProcessState;

typedef struct {
    pid_t pid;
    char command[max];
    MyProcessState state;
} MyProcess;

// Declare the MyProcess list array and the maximum number of MyProcesses
// #define MAX_MyProcessES 4096
extern MyProcess MyProcessList[max22];


void removeMyProcessFromList(int index);
MyProcess* get_myReqMyProcess(pid_t pid);
void addMyProcess(pid_t pid, const char *command, MyProcessState state) ;
void updateStateMyProcess();
void add_background_MyProcess(int pid, char*name);
const char* remove_background_MyProcess(pid_t pid) ;

void handle_background_MyProcesses();








#endif 












