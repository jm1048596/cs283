#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

char* removeExtraWS(char **list, char *string, int len, int entryNum) {
    int previousWS = 1;
    int newLen = 0;

    char *newString = (char*)malloc(strlen(string));
    if (newString == NULL) {
        perror("malloc failed");
        exit(-1);
    }
    
    for (int i = 0; i < len; i++) {
        if ((string[i] == ' ') || (string[i] == '\t')) {
            if (!previousWS) {
                newString[newLen] = ' ';
                newLen++;
                previousWS = 1;
            }
        } else {
            newString[newLen] = string[i];
            newLen++;
            previousWS = 0;
        }
        
    }
    if (newString[strlen(newString) - 1] == ' ') {
        newString[strlen(newString) - 1] = '\0';
    } else {
        newString[strlen(newString)] = '\0';
    }

    free(list[entryNum]);
    return newString;
}

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    char *curCom;
    char *split;
    char *clistTemp[CMD_MAX];

    clist->num = 0;

    curCom = strtok(cmd_line, PIPE_STRING);

    //load the individual commands into an array
    while (curCom != NULL) {
        if (clist->num == CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }
        if (strlen(curCom) > SH_CMD_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        clistTemp[clist->num] = strdup(curCom);
        if (clistTemp[clist->num] == NULL) {
            perror("strdup failed");
            exit(-1);
        }
        clist->num++;

        curCom = strtok(NULL, PIPE_STRING);
    }

    for (int i = 0; i < clist->num; i++) {
        clistTemp[i] = removeExtraWS(clistTemp, clistTemp[i], strlen(clistTemp[i]), i);
        if (strlen(clistTemp[i]) == 0) {
            clist->num--;
            continue;
        }
        printf("%s\n\n", clistTemp[i]);

        split = strchr(clistTemp[i], SPACE_CHAR);

        if (split == NULL) {
            strcpy(clist->commands[i].exe, clistTemp[i]);
            clist->commands[i].exe[strlen(clistTemp[i])] = '\0';

            printf("%s\n", clist->commands[i].exe);
        } else {
            strncpy(clist->commands[i].exe, clistTemp[i], split-clistTemp[i]);
            clist->commands[i].exe[split-clistTemp[i]] = '\0';

            strcpy(clist->commands[i].args, (split+1));
            clist->commands[i].args[strlen(clist->commands[i].args)] = '\0';

            printf("%s: %s\n",  clist->commands[i].exe, clist->commands[i].args);
        }

        free(clistTemp[i]);
    }

    return OK;
}