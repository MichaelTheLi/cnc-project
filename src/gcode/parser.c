//
// Created by Michael Lipinski on 17/01/2018.
//

#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include <math.h>

GCodeCommand *createCommand() {
    static float command[COMMAND_SIZE] = { };
    for (int i=0; i<COMMAND_SIZE; ++i) command[i] = NAN;

    static GCodeCommand commandReal = (GCodeCommand) command;
    return &commandReal;
}

float processToken(char *token) {
    char blockName = token[0];
    if (blockName == ';') { // Block is a comment
        return NAN;
    }
    if (blockName == '(') { // Block is a comment
        strtok(NULL, ")");
        return NAN;
    }

    return atof(token + 1);
}

unsigned char parseString(char *gcodeLine, GCodeCommand *command) {
    const char delim[2] = " ";

    char *line = strdup(gcodeLine);
    char *token = strtok(line, delim);
    while( token != NULL ) {
        float value = processToken(token);
        if (isnan(value)) {
            token = strtok(NULL, delim);
            continue;
        }

        char blockName = token[0];
        (*command)[blockName - 'A'] = value;

        token = strtok(NULL, delim);
    }

    free(line);

    return 1;
}
