//
// Created by Michael Lipinski on 17/01/2018.
//

#ifndef CLION_AVR_PARSER_H
#define CLION_AVR_PARSER_H

#define COMMAND_SIZE 26

typedef float *GCodeCommand;

#define COMMAND_INDEX(X) ((X) - 'A')
#define CHAR_FROM_INDEX(X) ((X) + 'A')

GCodeCommand *createCommand();
unsigned char parseString(char *gcodeLine, GCodeCommand *command);

#endif //CLION_AVR_PARSER_H
