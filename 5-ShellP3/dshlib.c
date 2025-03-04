#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

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

 int execute_pipeline(command_list_t *clist){//edited demo code
    int num_commands = clist->num;
    int pipes[num_commands - 1][2];  // Array of pipes
    pid_t pids[num_commands];        // Array to store process IDs

    // Create all necessary pipes
    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Create processes for each command
    for (int i = 0; i < num_commands; i++) {
        if (strcmp(clist->commands[i].argv[0],"cd")==0){
            if (clist->commands[i].argc!=2){//checking number of args
                printf("Error: Invalid cd command\n");
                continue;
            }
            else{
                if (chdir(clist->commands[i].argv[1]) != 0) {//checking for successful directory change
                    printf("Invalid directory \n");
                    continue;
                }
            }
        }
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) {  // Child process
            // Set up input pipe for all except first process
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }

            // Set up output pipe for all except last process
            if (i < num_commands - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Close all pipe ends in child
            for (int j = 0; j < num_commands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Execute command
            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process: close all pipe ends
    for (int i = 0; i < num_commands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all children
    for (int i = 0; i < num_commands; i++) {
        waitpid(pids[i], NULL, 0);
    }
    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    int argc = 0;
    char *arg_start = cmd_line;//pointer to the start

    for (int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;
    }

    while (*arg_start != '\0') {//removing leading whitespace
        while (*arg_start == SPACE_CHAR) {
            arg_start++;
        }

        if (*arg_start == '\0') {
            break;//reached end so end loop
        }

        char *arg_end = strchr(arg_start, SPACE_CHAR);//find next space

        if (arg_end == NULL) {//check if finished and reached end
            cmd_buff->argv[argc] = arg_start;
            argc++;
            break;
        } else {
            *arg_end='\0';//null terminate 
            cmd_buff->argv[argc]=arg_start;
            argc+=1;
            arg_start = arg_end + 1;//move to next start
        }

        if (argc >= CMD_ARGV_MAX) {//check number of args
            return ERR_TOO_MANY_COMMANDS;
        }
    }

    cmd_buff->argc = argc;
    
    if (argc == 0) {//no args
        return ERR_CMD_ARGS_BAD;
    }

    return OK;//no errors
}

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    char *com;//short for command
    clist->num = 0;

    com = strtok(cmd_line, PIPE_STRING);//split by pipe

    while (com != NULL && clist->num < CMD_MAX) {
        cmd_buff_t *cmd = &clist->commands[clist->num];

        int rc = build_cmd_buff(com, cmd);//build each buffer
        if (rc != OK) {
            if (rc == ERR_TOO_MANY_COMMANDS){
                return ERR_TOO_MANY_COMMANDS;
            }
            return rc;
        }

        clist->num += 1;
        com = strtok(NULL, PIPE_STRING);//next command
    }

    if (clist->num == 0) {
        return WARN_NO_CMDS;//no commands
    }
    return OK;//no errors
}


int exec_cmd(cmd_buff_t *cmd) {
    if (strcmp(cmd->argv[0],"cd")==0){
        if (cmd->argc!=2){//checking number of args
            printf("Error: Invalid cd command\n");
            return ERR_EXEC_CMD;
        }
        else{
            if (chdir(cmd->argv[1])!=0) {//checking for successful directory change
                printf("Invalid directory \n");
                return ERR_EXEC_CMD;
            }
        }
    }
    int rc = fork();//for the result of the fork
    if (rc < 0) {//check for errors in fork
        perror("fork error");
        return ERR_MEMORY;//return error
    }

    if (rc == 0) { 
        //The child will now exec, basically shape shifting itself
        if (execvp(cmd->argv[0],cmd->argv)<0){
            exit(ERR_EXEC_CMD);//error executing command
        }
    } else {
        int statC=0;//status of child
        waitpid(rc,&statC,0);//wait for child
        if (WIFEXITED(statC)) {
            return WEXITSTATUS(statC);//child exit status
        } else {
            return ERR_EXEC_CMD;//if there is an error
        }
    }
    return OK;//no error
}

int exec_local_cmd_loop()
{
    char *cmd_buff;
    cmd_buff = malloc(ARG_MAX);

    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        // Remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // Build the command list
        command_list_t clist;
        memset(&clist, 0, sizeof(command_list_t));

        if (build_cmd_list(cmd_buff, &clist) != OK) {
            printf(CMD_WARN_NO_CMD);
            continue;
        }

        if (clist.num > 1) {
            //printf("0");
            if (execute_pipeline(&clist) != OK) {
                //printf("1");
            }
        } else {
            // Handle single command (no pipes)
            if (exec_cmd(&clist.commands[0]) != OK) {
                //printf("2");
            }
        }
    }

    free(cmd_buff);//free
    return OK;//no error
}

