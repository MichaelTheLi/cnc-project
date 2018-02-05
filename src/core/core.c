//
// Created by Michael Lipinski on 22/01/2018.
//

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include "core.h"

AxisState initializeAxis(StepperState stepState, float stepSize);

CNCPosition* initializeCNCPosition() {
    InnerStepperState state1 = {};
    unsigned char microsteppingLevel = 4;
    state1.h_state = createStepper_hw(
            FULL_STEP_DOUBLE_PHASE,
            &PORTA,
            PA1, // STEP pin
            PA2, // DIR pin
            255, // ms1 pin
            255, // ms2 pin
            255  // ms3 pin
    );
    AxisState xAxis = initializeAxis(
            initiateStepper(
                    hardware_polulu,
                    state1
            ),
            0.15 / microsteppingLevel
    );
    InnerStepperState state2 = {};
    state2.h_state = createStepper_hw(
            FULL_STEP_DOUBLE_PHASE,
            &PORTA,
            PA3, // STEP pin
            PA4, // DIR pin
            255, // ms1 pin
            255, // ms2 pin
            255  // ms3 pin
    );
    AxisState yAxis = initializeAxis(
            initiateStepper(
                    hardware_polulu,
                    state2
            ),
            0.15 / microsteppingLevel
    );
    InnerStepperState state3 = {};
    state3.h_state = createStepper_hw(
            FULL_STEP_DOUBLE_PHASE,
            &PORTA,
            PA5, // STEP pin
            PA6, // DIR pin
            255, // ms1 pin
            255, // ms2 pin
            255  // ms3 pin
    );
    AxisState zAxis = initializeAxis(
            initiateStepper(
                    hardware_polulu,
                    state3
            ),
            0.15 / microsteppingLevel
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

AxisState initializeAxis(StepperState stepState, float stepSize) {
    return (AxisState) {
            .pos = 0.0,
            .stepState = stepState,
            .stepSize = stepSize,
    };
}