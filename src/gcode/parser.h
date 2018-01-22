//
// Created by Michael Lipinski on 17/01/2018.
//

#ifndef CLION_AVR_PARSER_H
#define CLION_AVR_PARSER_H

#define COMMAND_SIZE 26

typedef float *GCodeCommand;
typedef enum { gcode_parse_success, gcode_parse_empty, gcode_parse_error } GCodeParseResult;

#define COMMAND_INDEX(X) ((X) - 'A')
#define COMMAND_NAME_FROM_INDEX(X) ((X) + 'A')

GCodeCommand *createCommand();
GCodeParseResult parseString(char *gcodeLine, GCodeCommand *command);

#endif //CLION_AVR_PARSER_H
