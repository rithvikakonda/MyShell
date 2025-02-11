#include "headers.h"
#include "process.h"
extern int MyProcessCount;
extern int foregrnd_id;
extern int bg_count;
extern pid_t background_pids[max];
extern char background_names[max][max];

void removeMyProcessFromList(int ind) {
    for (int j = ind; j < MyProcessCount - 1; j++) {
        MyProcessList[j] = MyProcessList[j + 1];
    }
    MyProcessCount--; 
}

MyProcess* get_myReqMyProcess(pid_t pid){
  for(int i=0;i<MyProcessCount;i++){
    if(MyProcessList[i].pid == pid){
      return &MyProcessList[i];
    }
  }
  return NULL;
}
void addMyProcess(pid_t pid, const char *command, MyProcessState state) {
    if (MyProcessCount >= max22) {
        return;
    }
        MyProcessList[MyProcessCount].pid = pid;
        strncpy(MyProcessList[MyProcessCount].command, command, max);
        MyProcessList[MyProcessCount].state = state;
        MyProcessCount++;
    
}


void updateStateMyProcess() {
    for (int i = 0; i < MyProcessCount; i++) {
        int stsPss;
        pid_t result = waitpid(MyProcessList[i].pid, &stsPss, WNOHANG | WUNTRACED | WCONTINUED);

        if (result == 0) {
            continue;
        } else if (result > 0) {
            if (WIFSTOPPED(stsPss)) {
                MyProcessList[i].state = Stopped;
            } else if (WIFCONTINUED(stsPss)) {
                MyProcessList[i].state = Running;
            } else if (WIFEXITED(stsPss) || WIFSIGNALED(stsPss)) {
                // MyProcess exited or was killed, remove it from the list
                printf("MyProcess %d (%s) finished.\n", MyProcessList[i].pid, MyProcessList[i].command);
                if (MyProcessList[i].pid == foregrnd_id) {
                    printf("Foreground MyProcess %d has completed.\n", foregrnd_id);
                    foregrnd_id = -1;

                }
                removeMyProcessFromList(i);
                i--; // Decrement i because the list size has decreased
            }
        } else if (result == -1) {
            if (errno == ECHILD) {
                
                removeMyProcessFromList(i);
                i--; // Decrement i because the list size has decreased
            } else if (errno == ESRCH) {
                
                removeMyProcessFromList(i);
                i--; // Decrement i because the list size has decreased
            } else {
                // Some other error occurred
                perror("waitpid failed");
            }
        }
    }
}


void add_background_MyProcess(int pid, char*name) {
    if (bg_count < max22) {
        background_pids[bg_count] = pid;
        strncpy(background_names[bg_count], name, MAX_NAME_LENGTH - 1);
        background_names[bg_count][MAX_NAME_LENGTH - 1] = '\0';  // Ensure null-termination
        bg_count ++;
    } else {
        printf("Error: Maximum number of background MyProcesses reached.\n");
    }
}

const char* remove_background_MyProcess(pid_t pid) {
    for (int i = 0; i < bg_count; i++) {
        if (background_pids[i] == pid) {
            const char* name = background_names[i];
            for (int j = i; j < bg_count - 1; j++) {
                background_pids[j] = background_pids[j + 1];
                strncpy(background_names[j], background_names[j + 1], max);
            }

            bg_count--;
            return name;
        }
    }
    return NULL;  // PID not found
}


void handle_background_MyProcesses() {
    int stsPss;
    pid_t pid;

    while ((pid = waitpid(-1, &stsPss, WNOHANG)) > 0) {
       const char* MyProcess_name = remove_background_MyProcess(pid);
        if (WIFEXITED(stsPss)) {
           printf("%s with PID %d exited normally\n",
                       MyProcess_name, pid);
           
        } else if (WIFSIGNALED(stsPss)) {
                      
                      printf("%s with PID %d terminated abnormally\n",
                       MyProcess_name, pid);

        }
    }
}

















