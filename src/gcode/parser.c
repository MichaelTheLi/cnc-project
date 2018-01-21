//
// Created by Michael Lipinski on 17/01/2018.
//

#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

GCodeCommand *createCommand() {
    static float command[COMMAND_SIZE] = { 0 };

    static GCodeCommand commandReal = (GCodeCommand) command;
    return &commandReal;
}

unsigned char parseString(char *gcodeLine, GCodeCommand *command) {
    const char delim[2] = " ";

    char *line = strdup(gcodeLine);
    char *token = strtok(line, delim);
    while( token != NULL ) {
        char blockName = token[0];
        if (blockName == '(') { // Block is a comment
            token = strtok(NULL, delim);
            continue;
        }
        float value = atof(token+1);
        (*command)[blockName - 'A'] = value;

        token = strtok(NULL, delim);
    }

    free(line);

    return 1;
}
