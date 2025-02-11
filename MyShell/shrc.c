#include "headers.h"
#include "shrc.h"

#define max_alias 100
#define MaxLength 4096

struct alias{
  char aliasPrev[MaxLength];
  char aliasActual[MaxLength];
};


typedef struct alias alias;

alias ListAlias[max_alias];

int alCnt = 0;


void strip_quotes_and_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
        len--;
    }
    
    if (len > 1 && str[0] == '"' && str[len - 1] == '"') {
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }
}

void execute_shrc() {
    FILE *fp = fopen("myshrc", "r");
    if (fp == NULL) {
        printf("Unable to open File\n");
        return;
    }

    char sentC[MaxLength];

    while (fgets(sentC, sizeof(sentC), fp) != NULL) {
        if (sentC[0] == '#' || sentC[0] == '\n') {
            continue;
        }

        if (strstr(sentC, "alias ") == sentC || strchr(sentC, '=') != NULL) {
            char *aliasName;
            char *aliasValue;

            if (strstr(sentC, "alias ") == sentC) {
                aliasName = strtok(sentC + 6, "="); 
            } else {
                aliasName = strtok(sentC, "="); 
            }

            aliasValue = strtok(NULL, "\n");

            if (aliasValue) {
                strip_quotes_and_newline(aliasValue);
            }

            if (aliasName && aliasValue) {
                strcpy(ListAlias[alCnt].aliasPrev, aliasName);
                strcpy(ListAlias[alCnt].aliasActual, aliasValue);
                alCnt++;
            }
        }

        
    }
    // printf("exited\n");

    fclose(fp);
}

void replace_aliases(char *command) {
    for (int i = 0; i < alCnt; i++) {
        if (strstr(command, ListAlias[i].aliasPrev) == command) {
            char temp[MaxLength];
            strcpy(temp, ListAlias[i].aliasActual);
            strcat(temp, command + strlen(ListAlias[i].aliasPrev));
            strcpy(command, temp);
            return;
        }
    }
}







