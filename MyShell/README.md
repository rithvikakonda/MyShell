### main function

#### on running make and ./a.out shell begins

##### SPEC 1

--- maximum input length for all inputs are below 6000
prompt.c file contains the part of printing the prompt ... shell runs from the main function by calling run_shell() function

#### SPEC 2 

tokenize is done as part of code , in main..c file 

background process : pid is printed as : pid : value_of_pid 

error commands are handled as "ERROR : 'command' is not a valid command"

#### SPEC 3

---- always absolute path is printed .
---- hop.c file contains the functions regarding this specification ..
hop without argument prints the home directory of shell

-- invalid folder is given reports statement as invalid argument

-- hop x_directory - --> if it is given then x_directory is taken as previous directory

-- Assumed hop ~ is the home directory from where the shell invoked.




#### SPEC 4

--- reveal command without arguments lists the all files and directories of current working directory

--- reveal .c contain all the functions used for this execution

-- q22 .if previous working directory dont exist then,prints message that previous working directory don't exist message and continues for next input


-- reveal lists all files and directories in lexicographical order and each line contains only one file / directory name

-- maximum of 1000 files can be present in directory 

--- works only on directories for -l flag

-- color coding for -l are done to only names of files and directories  (color coding done as provided in document)

#### SPEC 5

--- log.c contains the functions for this specifications

---- Used circular 2d array to store commands upto 15 and update it as new commands are added

---- In multiple commands as input , the input without log are directly stored,inputs with log are stored by removing log functions from input command.

---- log purge clears all commands from file and 2darray also

--- log execute is performed then the command is not stored in the file,if index given is out of range (out of no.of commands stored in it) prints statement regarding invalid number

-- if log is empty then it doesnt show anything , just prints the next prompt

--- erroneous commands are stored (i am storing the erroneous commands also )

--- I create a commands.txt file when the program runs , and all the commands are updated to my 2d array and then to file .... as needed i can use both to print the commands

#### spec 6

-- when >2s are taken by foreground sleep command then i append to prompt for only 1st prompt

-- if multiple commands of sleep are given then i append to a string and add it to the prompt with total time taken in the last of string
same as example:

 <JohnDoe@SYS:~ sleep : 5s ; sleep :3s ; 8s>

-- When erroneous command is run in background then it prints pid , and a statement that says erroneous command is given as input and background failed ... as execvp returns if it is failed

-- assuming background process are only system commands and not for commands implemented by us

--- Assuming that when interactive process is going on such as giving input in terminal,then the result of background process is not printed..

-- so basically pid of background process is printed and after the next input (some other input in terminal) is given it prints about the status of completion of background process with pid.

--- Maximum no.of process can run are upto 6000 as it can store upto that limit

-- this bg and fg are done in main.c with differentiation to background and foreground.
-- When background process completed ,the name of process with  pid is printed.when it started pid in the form [ pid ] is printed on terminal.

#### spec 7

-- for printing virtual memory used - i have printed it in bytes with a prefix of "Bytes"

-- implemented this func in proclore.c

-- if its executable is not present or not accessible the colon for it kept blank 

-- for executable path : I am printing the absolute path of that executable

#### spec 8

-- sk.c file contains code of this functionality

-- recursively go thorugh the files

-- If the required file or directory specified is not present then either it prints the message or goes to next print without printing anything . Assume if the given is not present then doesnt print anything.

--> Assumed it accepts full name of file when target is provided






###### PART B

#### spec 9

-- assumed length of command atmost 4096
-- assumed file can contain atmost 100 aliasses

-- only 4 commands are defined here , can be added if wanted

-- file name named as myshrc

-- shrc.c contain function to this execution



### spec 10 , spec 11 , spec 12

--> command is splited by pipes then redirection 
--> dup2 function is mostly used
--> if command causes error and contain redirection then it is printed in the file
--> hop .. | wc  will print as 0 0 0 as my  bash
--> for hop .. and some redirection to file , assumed  hop .. doesnt execute only the output which is printed on terminal goes to file 
--> commands in pipe are assumed  run sequentially
--> assumed & runs for the command preceding it only but in continuation to redirection to file the pid gets printed in the file itself with the output of that command and if continuation to & without seperator as ; then the succeding command's output also redirected to file
--> I am parsing the string by pipes and then performing redirection if it present and sending the commands for user defined and sys commands to my previously implemented bg_fg

--> execute.c and execute.h contain the required code for the implementation

--> I refered online site for piping functionality


### spec 13

--> process.c contain the helper functions for this implementation
--> I am using array of structures to store the details of process and performing all operations like add, remove , update

--> I am sorting according to pids of process
--> My implementation adds process with pid, state , and command main part name to processlist , This is added when i execute the command(background) ---  Background processes are added , foreground also added but removed immediately they terminate so this doesnt effect the overall functionality of function
--> activities displays only background processes 
--> My implementation also adds the erroneous command to list and displays when activities is run -- in the stopped state
--> can be terminated by using ping
--> I assume max 4096 process can be added to activities

--> I just store main part of command name like sleep , echo in processlist

### spec 14

--> For 4 signal the process state is updated to stopped , remaining can be deleted from activities if they are terminated

--> ctrl c does nothing if there is no foreground process is running

--> ctrl D is pressed it kills/terminates the all the process in activities , the status of them are not dispalyed , sometimes till all the background running process finishes(killed) it doesnt exit (sometimes).

--> ping.c contains code related to it
--> When a process say sleep is in stopped state in background , if we terminate the process by ping terminate signal number then it shows as process terminated abnormally.

### spec 15

--> fg.c , bg.c contain the functions related to this spec
--> when bg process is brought to fg then for sleep command i am assumed that it dont print in prompt 


### spec 16


---> assumed the function stops only x is pressed and no change for ctrl commands like ctrl z  , ctrl c etc

--> prints the most recently created process

--> function is implemented in neonate function call

### spec 17

--> error statements are user defined







----- I have used ai tools for checking the code for removing the leading and tailing space and about replacing \n with \0 , and used man pages for clarity














