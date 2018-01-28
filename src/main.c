/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include "bits.h"
#include "core/core.h"
#include "gcode/executor.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

GCodeCommand *createDemoMoveCommand(float x, float y);


int main(void)
{
    bit_set(DDRB, BIT(0));
    bit_set(DDRB, BIT(1));
    bit_set(DDRB, BIT(2));
    bit_set(DDRB, BIT(3));
    bit_set(DDRB, BIT(4));

    CNCPosition* cncPosition = initializeCNCPosition();

    for(;;){
        executeCommand(
            createDemoMoveCommand(50.0f, 100.0f),
            cncPosition
        );
        executeCommand(
            createDemoMoveCommand(100.0f, 0.0f),
            cncPosition
        );
        executeCommand(
            createDemoMoveCommand(0.0f, 75.0f),
            cncPosition
        );
        executeCommand(
            createDemoMoveCommand(100.0f, 75.0f),
            cncPosition
        );
        executeCommand(
            createDemoMoveCommand(0.0f, 0.0f),
            cncPosition
        );
    }
}

GCodeCommand *createDemoMoveCommand(float x, float y) {
    GCodeCommand *commandPtr = createCommand();
    GCodeCommand command = *commandPtr;

    command[COMMAND_INDEX('G')] = 1;
    command[COMMAND_INDEX('X')] = x;
    command[COMMAND_INDEX('Y')] = y;

    return commandPtr;
}

#pragma clang diagnostic pop