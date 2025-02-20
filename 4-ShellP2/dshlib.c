#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

int loadCMD(cmd_buff_t *cmd, char *buff, int len) {
    int previousWS = 1;     //var to keep track of whether or not the previous character was whitespace
    int inQuote = 0;        //var to keep track of whether or not we're currently inside quotes
    char tempString[SH_CMD_MAX];        //array to hold the argument before allocating space for it
    
    int argLen = 0;
    for (int i = 0; i < len; i++) {
        if (cmd->argc >= 8) {
            printf("Too many arguments");
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        if (buff[i] == '\"') {
            inQuote = !inQuote;
            continue;
        }
        if ((buff[i] == ' ') || (buff[i] == '\t')) {
            if (inQuote) {
                tempString[argLen] = ' ';
                argLen++;
                continue;
            }
            if (!previousWS) {
                tempString[argLen] = '\0';
                previousWS = 1;

                cmd->argv[cmd->argc] = malloc(argLen+1);
                if (cmd->argv[cmd->argc] == NULL) {
                    perror("malloc failed");
                    return ERR_MEMORY;
                }
                strncpy(cmd->argv[cmd->argc], tempString, argLen+1);
                argLen = 0;
                cmd->argc++;
            }
        } else {
            tempString[argLen] = buff[i];
            argLen++;
            previousWS = 0;
        }
        
    }
    //last command wasn't added because it didn't end in at least one space, so add it
    if (argLen != 0) {
        tempString[argLen] = '\0';

        cmd->argv[cmd->argc] = malloc(argLen+1);
        if (cmd->argv[cmd->argc] == NULL) {
            perror("malloc failed");
            return ERR_MEMORY;
        }
        strncpy(cmd->argv[cmd->argc], tempString, argLen+1);
        cmd->argc++;
    }

    return OK;
}

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char cmd_buff[ARG_MAX];
    int rc = 0;
    cmd_buff_t cmd;

    while(1){
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
           printf("\n");
           break;
        }
        //remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 
        //IMPLEMENT THE REST OF THE REQUIREMENTS
        //load the command
        cmd.argc = 0;
        cmd._cmd_buffer = cmd_buff;
        rc = loadCMD(&cmd, cmd_buff, strlen(cmd_buff));
        if (rc < 0) {
            return rc;
        }
        cmd.argv[cmd.argc] = 0;     //null terminator

        char *exe = cmd.argv[0];

        //built-in's
        if (strcmp(exe, EXIT_CMD) == 0) {
            break;
        }
        if (strcmp(exe, "dragon") == 0) {
            print_dragon();
            continue;
        }
        if (strcmp(exe, "cd") == 0) {
            if (cmd.argc > 1) {
                rc = chdir(cmd.argv[1]);
                //check if an error occured (usually that the directory wasn't found)
                if (rc < 0) {
                    printf("dsh: cd: %s: ", cmd.argv[1]);
                    fflush(stdout);
                    perror("");
                }
            }
            continue;
        }

        //fork/exec

        int f_result, c_result;

        f_result = fork();
        if (f_result < 0)
        {
            perror("fork error");
            exit(1);
        }

        //child will execute command
        if (f_result == 0)
        {
            rc = execvp(cmd.argv[0], cmd.argv);
            if (rc < 0)
            {
                perror("External command failed with message");
                exit(ERR_EXEC_CMD);
            }
        } 
        //parent waits for child to be done
        else {
            wait(&c_result);
        }

        //freedom
        for (int i=0; i<cmd.argc; i++) {
            free(cmd.argv[i]);
        }

    }

    return OK;
}