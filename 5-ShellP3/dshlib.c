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

//this function is pretty much entirely from the demo, thanks for that
int execute_pipeline(command_list_t *clist) {
    int numCommands = clist->num;
    int pipes[numCommands - 1][2];  // Array of pipes
    pid_t pids[numCommands];        // Array to store process IDs

    // Create all necessary pipes
    for (int i = 0; i < numCommands - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return EXIT_FAILURE;
        }
    }

    // Create processes for each command
    for (int i = 0; i < numCommands; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            return EXIT_FAILURE;
        }

        if (pids[i] == 0) {  // Child process
            // Set up input pipe for all except first process
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }

            // Set up output pipe for all except last process
            if (i < numCommands - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Close all pipe ends in child
            for (int j = 0; j < numCommands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Execute command
            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp");
            return EXIT_FAILURE;
        }
    }

    // Parent process: close all pipe ends
    for (int i = 0; i < numCommands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all children
    for (int i = 0; i < numCommands; i++) {
        waitpid(pids[i], NULL, 0);
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
    char *curCom;                   //to parse initial buffer
    char *clistTemp[CMD_MAX];       //to load all the commands in prior to individual parsing
    command_list_t clist;

    while(1){
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
           printf("\n");
           break;
        }
        //remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 
        //IMPLEMENT THE REST OF THE REQUIREMENTS
        clist.num = 0;      //num of commands

        curCom = strtok(cmd_buff, PIPE_STRING);

        //load the individual commands into an array
        while (curCom != NULL) {
            if (clist.num == CMD_MAX) {
                return ERR_TOO_MANY_COMMANDS;
            }
            if (strlen(curCom) > SH_CMD_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }

            clistTemp[clist.num] = strdup(curCom);
            if (clistTemp[clist.num] == NULL) {
                perror("strdup failed");
                exit(-1);
            }
            clist.num++;
            // cmd._cmd_buffer = cmd_buff;

            curCom = strtok(NULL, PIPE_STRING);
        }

        int emptyComms = 0;     //to accurately track the index in clist's commands to write to

        //then indivdually parse each command and store them into the clist
        for (int i = 0; i < clist.num; i++) {
            // cmd = malloc(sizeof(cmd_buff_t));
            // if (cmd == NULL) {
            //     perror("malloc failed");
            //     return ERR_MEMORY;
            // }

            cmd.argc = 0;       //num of args in a command
            rc = loadCMD(&cmd, clistTemp[i], strlen(clistTemp[i]));
            if (rc < 0) {
                return rc;
            }
            //empty command = ignore
            if (strlen(clistTemp[i]) == 0) {
                clist.num--;
                emptyComms++;
                continue;
            }
            
            cmd.argv[cmd.argc] = 0;     //null terminator

            clist.commands[i-emptyComms] = cmd;

            free(clistTemp[i]);
        }

        char *exe = clist.commands[0].argv[0];

        //built-in's
        if (strcmp(exe, EXIT_CMD) == 0) {
            break;
        }
        if (strcmp(exe, "dragon") == 0) {
            print_dragon();
            continue;
        }
        if (strcmp(exe, "cd") == 0) {
            if (clist.commands[0].argc > 1) {
                rc = chdir(clist.commands[0].argv[1]);
                //check if an error occured (usually that the directory wasn't found)
                if (rc < 0) {
                    printf("dsh: cd: %s: ", cmd.argv[1]);
                    fflush(stdout);
                    perror("");
                }
            }
            continue;
        }

        //external commands, just send 'em over
        execute_pipeline(&clist);

        //freedom
        for (int i=0; i<clist.num; i++) {
            for (int j=0; j<cmd.argc; j++) {
                free(clist.commands[i].argv[j]);
            }
            // free(clist.commands[i]);
        }
    }

    return OK;
}