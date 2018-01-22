//
// Created by Michael Lipinski on 22/01/2018.
//

#include <stdlib.h>
#include <string.h>
#include "core.h"

AxisState initializeAxis(StepState *stepState, float stepSize);

CNCPosition* initializeCNCPosition() {
    AxisState xAxis = initializeAxis(
            attachStepper(0, FULL_STEP_DOUBLE_PHASE),
            0.0001
    );
    AxisState yAxis = initializeAxis(
            attachStepper(1, FULL_STEP_DOUBLE_PHASE),
            0.0001
    );
    AxisState zAxis = initializeAxis(
            attachStepper(2, FULL_STEP_DOUBLE_PHASE),
            0.0007
    );

    CNCPosition cncPosition = {
            .x = xAxis,
            .y = yAxis,
            .z = zAxis,
            .feedRate = 10.1,
    };

    CNCPosition *cncPositionPtr = malloc(sizeof *cncPositionPtr);
    if (cncPositionPtr == NULL) {
        /* failed to allocate, handle error here */
    } else {
        /* OK to copy.  Make sure you initialize 'hold' to something valid */
        memcpy(cncPositionPtr, &cncPosition, sizeof *cncPositionPtr);
    }

    return cncPositionPtr;
}

AxisState initializeAxis(StepState *stepState, float stepSize) {
    return {
            .pos = 0.0,
            .stepState = stepState,
            .stepSize = stepSize,
    };
}