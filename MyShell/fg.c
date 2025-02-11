
#include "headers.h"
#include "fg.h"
#include "process.h"

extern int foregrnd_id;

void fgP(pid_t bpid) {
//   printf(RED"enterd"RESET"\n");
    int stat;
    MyProcess* process = get_myReqMyProcess(bpid);
    if (process == NULL) {
        printf(RED"Given pid is invalid"RESET"\n");
        return;
    }
   setpgid(bpid, getpgid(0));
    
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    // Give control of the terminal to the resumed process
    if (tcsetpgrp(STDIN_FILENO, bpid) < 0) {
        perror(RED"Error giving terminal control to process"RESET);
        return;
    }

    // Send the SIGCONT signal to continue the process if it was stopped
    if (kill(bpid, SIGCONT) < 0) {
        perror(RED"Error sending SIGCONT"RESET);
        return;
    }
   
    process->state = Running;

    foregrnd_id = bpid;
    // removeMyProcessFromList(foregrnd_id);
     waitpid(bpid, &stat, WUNTRACED);
      removeMyProcessFromList(foregrnd_id);
     foregrnd_id = -1;
   

    if (tcsetpgrp(STDIN_FILENO, getpgid(0)) < 0) {
        perror(RED"Error returning terminal control to shell"RESET);
    }

    // Restore the default behavior of terminal signals
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);

    return;
}
















