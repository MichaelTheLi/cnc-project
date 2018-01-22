//
// Created by Michael Lipinski on 22/01/2018.
//

#ifndef CLION_AVR_EXECUTOR_H
#define CLION_AVR_EXECUTOR_H

#include "./parser.h"
#include "../core/core.h"

typedef enum { gcode_execute_success, gcode_execute_common_error } GCodeExecuteResult;

GCodeExecuteResult executeCommand(GCodeCommand *gCodeCommand, CNCPosition *cncPosition);

#endif //CLION_AVR_EXECUTOR_H
