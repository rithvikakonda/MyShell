#include "headers.h"
#include "proclore.h"

#define s_size max

void funcproclore(int rr_pid){
   char req_path[max], str_buffer[max];
   int pro_grp;
    FILE *file_vth_status;
    
    char process_state[3] ;  
    char virtual_memory[s_size] ;  
    char exe_path[max] ;     

    snprintf(req_path, sizeof(req_path), "/proc/%d/status",rr_pid);

   file_vth_status = fopen(req_path, "r");
    if (file_vth_status == NULL) {
        printf(RED"Error: Process with pid %d not found."RESET"\n",rr_pid);
        return;
    }

 
    while (fgets(str_buffer, sizeof(str_buffer), file_vth_status)) {
        
        sscanf(str_buffer, "State:  %s", process_state);
        sscanf(str_buffer, "VmSize:  %s", virtual_memory);

    }
    fclose(file_vth_status);

    pro_grp = getpgid(rr_pid);
    
    if (pro_grp == -1) {
        printf("Error: Unable to get group ID for pid %d.\n", rr_pid);
        return;
    }
    
    int fg_pgid = tcgetpgrp(1);

   
    if (fg_pgid == pro_grp && process_state[0] == 'R' || process_state[0] == 'S' && fg_pgid == pro_grp ) {
        strcat(process_state, "+"); 
    }

    snprintf(req_path, sizeof(req_path), "/proc/%d/exe",rr_pid);  
    int len = readlink(req_path, exe_path, sizeof(exe_path) - 1);
    if (len != -1) {
        exe_path[len] = '\0';
    }
    int storage = atoi(virtual_memory);


    printf("pid : %d\n",rr_pid);
    printf("process status : %s\n", process_state);
     printf("Process Group : %d\n", pro_grp);
    printf("Virtual memory : %d Bytes\n", storage);

    printf("executable path : %s\n", exe_path);


}




















