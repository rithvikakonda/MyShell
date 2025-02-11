#include "headers.h"
void getStringUntilFirstWhitespace(const char *str, char *result) {
    int i = 0;
    
    while (str[i] != '\0' && str[i] != ' ' && str[i] != '\t') {
        result[i] = str[i];
        i++;
    }
    
    result[i] = '\0';
}

void execute_redirection(char* cmd_redtn) {
    char Red_Cmd[max] = {0};
    char Input_File[max] = {0};
    char Output_File[max] = {0};
    char My_cmd[max] = {0};
    char* next_file;

    cmd_redtn[strcspn(cmd_redtn, "\n")] = '\0';
    delete_leading_spaces(cmd_redtn);
    strcpy(Red_Cmd, cmd_redtn);
    strcpy(My_cmd, cmd_redtn);

    int Append_flag = 0;
    int input_Flag = 0;
    int output_Flag = 0;
    int ipIdx = -1;
    int otIdx = -1;
    int ApIdx = -1;
     int bg = 0;

    if (strstr(Red_Cmd, ">>") != NULL) {
        Append_flag = 1;
    }
    if (strstr(Red_Cmd, ">") != NULL || strstr(Red_Cmd, ">>") != NULL) {
      
        output_Flag = 1;
    }
    if (strstr(Red_Cmd, "<") != NULL) {
        input_Flag = 1;
    }
    char* command_part = strtok(Red_Cmd, "><");
    char* file_part = strtok(NULL, "><");
     if(strstr(file_part,"&")!= NULL){
           bg = 1;

        }
    int myf = 0;


    if( (input_Flag==1 && output_Flag==1) || (input_Flag==1 && Append_flag == 1)){
   
       myf = 1;
      for (int i = 0; i < strlen(cmd_redtn); i++) {
        if (cmd_redtn[i] == '<') {
            ipIdx = i;  // Store the position of input redirection
        } else if (cmd_redtn[i] == '>') {
            if (i + 1 < strlen(cmd_redtn) && cmd_redtn[i + 1] == '>') {
                ApIdx = i;  
                otIdx = i;  
                i++;  
            } else {
                otIdx = i;  
            }
        }
      }

      next_file = strtok(NULL,"><");
       if(strstr(next_file,"&")!= NULL){
           bg = 1;
        }

   
    }
  
    delete_leading_spaces(command_part);
    if (file_part != NULL) {
        file_part[strcspn(file_part, "\n")] = '\0';
        delete_leading_spaces(file_part);
        remove_trailing_spaces(file_part);
    }
    
    
    if(myf != 0){
       next_file[strcspn(next_file, "\n")] = '\0';
    delete_leading_spaces(next_file);
    remove_trailing_spaces(next_file);
    // printf("afterman :%s.\n",next_file);

    }
   

    strcpy(My_cmd, command_part);

    if(bg==1){
        char temp[max];
         getStringUntilFirstWhitespace(file_part, temp);
         strcpy(file_part,temp);
        //  printf("file:%s.\n",file_part);
         if(myf == 1){
            getStringUntilFirstWhitespace(next_file, temp);
         strcpy(next_file,temp);
        //  printf("file:%s.\n",next_file);

         }

    }

    if (input_Flag == 1) {
        if(myf == 0){
        strcpy(Input_File, file_part);
        }else{
         if((otIdx != -1 || ApIdx != -1) && (ipIdx < otIdx || ipIdx < ApIdx)){
            strcpy(Input_File,file_part);
         }else{
            strcpy(Input_File,next_file);
         }
        }
        //  printf("ip_file :%s.\n", Input_File);
        
    } 
    // printf("nextf:%s\n",next_file);
     if (output_Flag == 1) {
        // printf("11\n");
        if(myf==0){
            // printf("otf!!\n");
        strcpy(Output_File, file_part);
        }else{
            if(ipIdx != -1 && otIdx < ipIdx ){
                strcpy(Output_File,file_part);
            }else{
                
                strcpy(Output_File,next_file);
            }
        }
      
    }

    Input_File[strcspn(Input_File, "\n")] = '\0';

    // char* Arg_mine[max] = {0}; // Argument list for execvp
    // int arg_count = 0;

    // char* token = strtok(My_cmd, " ");
    // while (token != NULL) {
    //     Arg_mine[arg_count++] = token;
    //     token = strtok(NULL, " ");
    // }
    // Arg_mine[arg_count] = NULL; 

    // Save original file descriptors
    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stdin = dup(STDIN_FILENO);

    pid_t pid = fork();
      int fd_output;
      int fd_Input;
    if (pid == 0) { // Child process

        if (input_Flag == 1) {

            fd_Input = open(Input_File, O_RDONLY);
            if (fd_Input < 0) {
                // printf("input\n");
                perror(RED"No such input file found!"RESET);
                exit(1);
            }
            dup2(fd_Input, STDIN_FILENO);
            close(fd_Input);
        }


        if (output_Flag == 1) {

            if (Append_flag == 1) {
                fd_output = open(Output_File, O_WRONLY | O_CREAT | O_APPEND, 0644);
            } else {
                fd_output = open(Output_File, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            if (fd_output < 0) {
 
                perror(RED"Failed to open output file!"RESET);
                exit(1);
            }
            dup2(fd_output, STDOUT_FILENO);
            close(fd_output);
        }

        if (strstr(command_part, "&") != NULL) {
            bg = 1;
        }
        if(strstr(file_part,"&")!= NULL){
            bg = 1;

        }
        if(myf == 1){
            if(strstr(next_file,"&")!= NULL){
            bg = 1;

        }

        }
        if(bg == 1){
            strcat(command_part," &");
            // printf("newcmd:%s.\n",command_part);
        }
        if(output_Flag){
        bg_fg(bg, -1, command_part,saved_stdout,fd_output);

        }
        else if (input_Flag){

        bg_fg(bg, -1, command_part,saved_stdout,fd_Input);
        }
        
        else{
        bg_fg(bg, -1, command_part,saved_stdout,fd_output);
        }

        exit(0);

    } else if (pid > 0) { // Parent process
        wait(NULL); // Wait for child to complete
    } else {
        perror("fork failed");
    }


    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stdin, STDIN_FILENO);


    close(saved_stdout);
    close(saved_stdin);
}

  void execute_PipeLine(char* CMD) {
    char* pp_Cmds[max]; // To hold the pipeline commands
    int np = 0; // Number of pipe-separated commands

   
    CMD[strcspn(CMD, "\n")] = '\0'; 
    
    // Tokenize by pipe symbol
    char* token = strtok(CMD, "|");
    while (token != NULL) {
        pp_Cmds[np] = token;
        token = strtok(NULL, "|");
        np++;
    }

    // Number of pipe-separated commands
    int cmd_cnt = np;
    int pipes[cmd_cnt - 1][2];

    // Create pipes
    for (int i = 0; i < cmd_cnt - 1; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("pipe error");
            exit(1);
        }
    }

    pid_t pids[cmd_cnt]; // To hold the process IDs

    for (int cmd_indx = 0; cmd_indx < cmd_cnt; cmd_indx++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork error");
            exit(1);
        }

        if (pid == 0) { // Child process
           
            if (cmd_indx > 0) {
                // Redirect input to the read end of the previous pipe
                dup2(pipes[cmd_indx - 1][0], STDIN_FILENO);
            }

           
            if (cmd_indx < cmd_cnt - 1) {
                // Redirect output to the write end of the current pipe
                dup2(pipes[cmd_indx][1], STDOUT_FILENO);
            }

            // Close all pipe file descriptors
            for (int i = 0; i < cmd_cnt - 1; i++) {
                close(pipes[i][0]);
                close(pipes[i][1]);
            }

            // Handle redirection if needed
            if (strstr(pp_Cmds[cmd_indx], ">") != NULL || 
                strstr(pp_Cmds[cmd_indx], ">>") != NULL || 
                strstr(pp_Cmds[cmd_indx], "<") != NULL) {
                
               
                execute_redirection(pp_Cmds[cmd_indx]); // Output redirection

            } else {
                
                int bg = (strstr(pp_Cmds[cmd_indx], "&") != NULL) ? 1 : 0;
                // printf("bg :%d\n",bg);
                bg_fg(bg, -1, pp_Cmds[cmd_indx],-1,-1); // Execute command (fg/bg)
            }

            exit(0); // Child process exits after executing the command
        } else { // Parent process: store the child process ID
            pids[cmd_indx] = pid;
        }
    }

    // Close all pipe file descriptors in the parent process
    for (int i = 0; i < cmd_cnt - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < cmd_cnt; i++) {
        waitpid(pids[i], NULL, 0);
    }
}















