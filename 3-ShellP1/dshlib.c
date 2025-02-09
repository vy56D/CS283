#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

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
    char *parse;
    clist->num = 0;

    parse = strtok(cmd_line, PIPE_STRING);

    while (parse != NULL) {
        if (clist->num >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS; 
        }

        while (isspace((unsigned char)*parse)) {
            parse++;
        }

        char *end = parse + strlen(parse) - 1;
        while (end > parse && isspace((unsigned char)*end)) {
            end--;
        }
        *(end + 1) = '\0';

        if (strlen(parse) == 0) {
            parse = strtok(NULL, PIPE_STRING);
            continue;
        }

        command_t *com = &clist->commands[clist->num];
        memset(com, 0, sizeof(command_t));
        memset(com->args, 0, ARG_MAX);
        memset(com->exe, 0, EXE_MAX);

        char *firstSpace = strchr(parse, SPACE_CHAR);
        if (firstSpace != NULL){
            *firstSpace = '\0';
            firstSpace+=1;
        }
        if (strlen(parse) > EXE_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
        strcpy(com->exe, parse);

         if (firstSpace != NULL && *firstSpace != '\0') {
            if (strlen(firstSpace) > ARG_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            strcpy(com->args, firstSpace);
        }

        clist->num += 1;

        parse = strtok(NULL, PIPE_STRING); 
    }
    return OK;
}