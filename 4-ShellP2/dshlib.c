#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"
#include <errno.h>

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

int exec_local_cmd_loop() {
    int child =0;
    char *cmd_buff;
    cmd_buff_t cmd;
    cmd_buff= malloc(ARG_MAX);
    cmd._cmd_buffer = malloc(SH_CMD_MAX);

    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }
        //remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
    
        // Initialize the command structure
        cmd._cmd_buffer = cmd_buff;//store command
        cmd.argc = 0;
    
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
            child = WARN_NO_CMDS;
            continue;
        }
    
        char *begin = cmd_buff;
        i = 0;//reset i
        int quotes = 0; //acts as a boolean
    
        while (cmd_buff[i] != '\0') {
            if (cmd_buff[i] == '"') {
                quotes = !quotes;//flip
                if (!quotes) {
                    cmd_buff[i] = '\0';//null terminate and save into struct
                    cmd.argv[cmd.argc] = begin;
                    cmd.argc++;
                    begin = &cmd_buff[i + 1];
                } else {
                    // Start of quoted argument
                    begin = &cmd_buff[i + 1];//mark new beginning
                }
                i++;
            } else if (cmd_buff[i] == SPACE_CHAR && !quotes) {//not in quotes but has a space
                cmd_buff[i] = '\0';
                if (begin != &cmd_buff[i]) {
                    cmd.argv[cmd.argc] = begin;//save arg
                    cmd.argc++;
                }
                begin = &cmd_buff[i + 1];//set new beginning
                i++;
            } else {//increment
                i++;
            }
    
            if (cmd.argc >= CMD_ARGV_MAX) {//checking to see if there are too many args
                printf(CMD_ERR_PIPE_LIMIT, CMD_ARGV_MAX);
                break;
            }
        }
    
        if (begin != &cmd_buff[i] && cmd.argc < CMD_ARGV_MAX) {//getting last arg
            cmd.argv[cmd.argc] = begin;
            cmd.argc++;
        }
    
        if (quotes) {//checking for missing end quotes
            printf("Error: Missing end quotes \n");
            continue;
        }

        if (cmd.argc > 0){//argument size check
            if (strcmp(cmd.argv[0], EXIT_CMD) == 0) {//checking for exit command
                exit(EXIT_SUCCESS);
            }
            if (strcmp(cmd.argv[0], "cd") == 0) {//checking for cd
                if (cmd.argc == 1) {
                    continue;
                }
                if (cmd.argc > 2) {
                    continue;
                }
                if (chdir(cmd.argv[1]) != 0) {//checking for successful directory change
                    printf("Invalid directory \n");
                    child = ENOTDIR;
                    continue;
                }
                continue;
            }
            if (strcmp(cmd.argv[0], "rc") == 0){//rc command
                printf("%d",child);
                continue;
            }
        }
        
        int f_result, c_result;
        cmd.argv[cmd.argc] = NULL;
        

        f_result = fork();
        if (f_result < 0){
            perror("fork error");
            exit(errno);
        }

        if (f_result == 0){
            //The child will now exec, basically shape shifting itself
           // int rc = execvp(cmd.argv[0], cmd.argv);
            int rc = execvp(cmd.argv[0], cmd.argv);
            if (rc < 0){
                if (errno == ENOENT){//checking errnos
                    printf("Error: File not found");
                    
                }
                else if (errno == EACCES){
                    printf("Error: Permission denied");
                    
                }
                else if (errno == ENOTDIR){
                    printf("Error: Not a directory");
                    
                }
                else if (errno == EISDIR){
                    printf("Error: Is a directory");
                    
                }
                else if (errno == EIO){
                    printf("Error: Input or output error");
    
                }
                else if (errno == ENOEXEC){
                    printf("Error: Invalid or unsupported executable or format");
                    
                }  
                else{
                    printf("Error executing command");
                    
                }
                exit(errno);
                
                //we need to handle the error to make sure execution does
                //not continue, or likely wierd things will happen. 
                
                            //here if the fork failed
            }
        } else{
            wait(&c_result);//wait for child process
            child = WEXITSTATUS(c_result);
            continue;
        }
    }
    free(cmd_buff);//free memory
    return child;
}


    // TODO IMPLEMENT MAIN LOOP

    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing

    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"