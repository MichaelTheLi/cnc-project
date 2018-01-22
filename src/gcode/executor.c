//
// Created by Michael Lipinski on 22/01/2018.
//

#include "executor.h"
#include "math.h"

void executeLinearMovement(float x, float y, float z, CNCPosition *cncPosition);
void moveAxisLinear(AxisState *axisState, float newPos);

GCodeExecuteResult executeCommand(GCodeCommand *gCodeCommand, CNCPosition *cncPosition) {
    GCodeCommand command = *gCodeCommand;

    float x = command[COMMAND_INDEX('X')];
    float y = command[COMMAND_INDEX('Y')];
    float z = command[COMMAND_INDEX('Z')];

    if (command[COMMAND_INDEX('G')] == 1) {
        if (isnan(x) && isnan(y) && isnan(z)) {
            return gcode_execute_common_error; // TODO Make error more specific
        }

        executeLinearMovement(x, y, z, cncPosition);
    }

    return gcode_execute_success;
}

void executeLinearMovement(float x, float y, float z, CNCPosition *cncPosition) {
    moveAxisLinear(&cncPosition->x, x);
    moveAxisLinear(&cncPosition->y, y);
    moveAxisLinear(&cncPosition->z, z);
}

void moveAxisLinear(AxisState *axisState, float newPos) {
    StepState *stepState = axisState->stepState;

    float moved = 0.0f;
    float perStep = axisState->stepSize;
    float dx = newPos - axisState->pos;

    setDirectionByPtr(stepState, dx < 0 ? CW : CCW);

    while(moved < fabs(dx)) {
        makeStepByPtr(stepState);

        moved += perStep;
    }

    axisState->pos += dx < 0
        ? -moved
        : moved;
}