#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

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
                    printf("Error: Invalid directory \n");
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

int build_cmd_buff(char *cmd_buff, cmd_buff_t *cmd) {//edited 4-Shell
        cmd->_cmd_buffer = cmd_buff;//store command
        cmd->argc = 0;
    
        int count = 0;
        while (cmd_buff[count] == SPACE_CHAR) {//counting number of spaces to shift
            count++;
        }
        int i = 0;
        while (cmd_buff[i + count] != '\0') {//shifting/removing leading whitespace
            cmd_buff[i] = cmd_buff[i + count];
            i++;
        }
        cmd_buff[i] = '\0';
        int len = strlen(cmd_buff);
        while (len >= 1 && cmd_buff[len - 1] == SPACE_CHAR) {//remove trailing whitespace
            cmd_buff[len - 1] = '\0';
            len--;
        }
    
        if (strlen(cmd_buff) == 0) {//check for no input
            printf(CMD_WARN_NO_CMD);
            return WARN_NO_CMDS;
        }
    
        char *begin = cmd_buff;
        i = 0;//reset i
        int quotes = 0; //acts as a boolean
    
        while (cmd_buff[i] != '\0') {
            if (cmd_buff[i] == '"') {
                quotes = !quotes;//flip
                if (!quotes) {
                    cmd_buff[i] = '\0';//null terminate and save into struct
                    cmd->argv[cmd->argc] = begin;
                    cmd->argc++;
                    begin = &cmd_buff[i + 1];
                } else {
                    // Start of quoted argument
                    begin = &cmd_buff[i + 1];//mark new beginning
                }
                i++;
            } else if (cmd_buff[i] == SPACE_CHAR && !quotes) {//not in quotes but has a space
                cmd_buff[i] = '\0';
                if (begin != &cmd_buff[i]) {
                    cmd->argv[cmd->argc] = begin;//save arg
                    cmd->argc++;
                }
                begin = &cmd_buff[i + 1];//set new beginning
                i++;
            } else {//increment
                i++;
            }
    
            if (cmd->argc >= CMD_ARGV_MAX) {//checking to see if there are too many args
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
        }
    
        if (begin != &cmd_buff[i] && cmd->argc < CMD_ARGV_MAX) {//getting last arg
            cmd->argv[cmd->argc] = begin;
            cmd->argc++;
        }
    
        if (quotes) {//checking for missing end quotes
            printf("Error: Missing end quotes \n");
            return ERR_EXEC_CMD;
        }
    return OK;//no errors
}

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    char *com;//short for command
    clist->num = 0;

    com = strtok(cmd_line, PIPE_STRING);//split by pipe

    while (com != NULL) {
        if (clist->num > CMD_MAX) {
            printf(CMD_ERR_PIPE_LIMIT,CMD_MAX);
            return ERR_TOO_MANY_COMMANDS;//too many commands
        }

        cmd_buff_t *cmd = &clist->commands[clist->num];

        int rc = build_cmd_buff(com, cmd);//build each buffer
        if (rc != OK) {
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
                printf("Error: Invalid directory \n");
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

    while(1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        // Remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        command_list_t clist;
        memset(&clist, 0, sizeof(command_list_t));

        if (build_cmd_list(cmd_buff, &clist) != OK) {//build command list
            continue;
        }

        if (clist.num > 1) {//has many commands
            //printf("0");//debug
            if (execute_pipeline(&clist) != OK) {
                //printf("1");//debug
            }
        } else {
            if (exec_cmd(&clist.commands[0]) != OK) {//one command
                //printf("2");//debug
            }
        }
    }
    free(cmd_buff);//free
    return OK;//no error
}

