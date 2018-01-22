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
    if (blockName == '(') { // Block is a comment
        strtok(NULL, ")");
        return NAN;
    }

    return atof(token + 1);
}

GCodeParseResult parseString(char *gcodeLine, GCodeCommand *command) {
    GCodeParseResult result = gcode_parse_empty;

    const char delim[2] = " ";

    char *line = strdup(gcodeLine);
    char *token = strtok(line, delim);
    while( token != NULL ) {
        char blockName = token[0];
        if (blockName == ';') { // Semicolon comment, terminate
            break;
        }
        float value = processToken(token);
        if (isnan(value)) {
            token = strtok(NULL, delim);
            continue;
        }

        (*command)[blockName - 'A'] = value;
        if (result == gcode_parse_empty) {
            result = gcode_parse_success;
        }

        token = strtok(NULL, delim);
    }

    free(line);

    return result;
}
