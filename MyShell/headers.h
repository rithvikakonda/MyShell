#ifndef  header_h
#define headers_h


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>   // For getcwd() and chdir()
#include <limits.h>
#include <sys/socket.h>
#include <netdb.h>   // For PATH_MAX
#include <sys/wait.h>
#include <pwd.h>      // For getpwuid() to get username
#include <sys/types.h> // For getuid() to get user ID
#include <string.h>   // For strcmp()
#include <errno.h>
#include <grp.h>
#include <time.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <dirent.h> 
#include<termios.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include "hop.h"
#include "prompt.h"
#include "reveal.h"
#include "log.h"
#include "proclore.h"
#include "sk.h"
#include "ping.h"
#include "process.h"
#include "fg.h"
#include "bg.h"
#include "execute.h"
#include "iman.h"
#include "shrc.h"




#define max 6000
#define max1 6000
#define my_file "commands_file"
#define ncmd 15  // no.of history commands
#define cmdlen 6000 // length of each cmd
#define max22 4096


#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_WHITE "\033[0;37m"
#define COLOR_BLUE  "\033[0;34m"
#define RED "\033[31m"
#define RESET "\033[0m"
#define MAX_PROCESSES max    // Maximum number of background processes
#define MAX_NAME_LENGTH max 



void tokenize_cmd(char*command);
void run_commands();
void delete_leading_spaces(char* sttr);
void remove_trailing_spaces(char *str);

void bg_fg(int bg,int j,char* CMMD,int saved_stdout,int fd_output);




#endif
