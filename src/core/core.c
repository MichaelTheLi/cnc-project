//
// Created by Michael Lipinski on 22/01/2018.
//

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include "core.h"

AxisState initializeAxis(StepperState stepState, float stepSize);

CNCPosition* initializeCNCPosition() {
    AxisState xAxis = initializeAxis(
            initiateStepper(
                    hardware_polulu,
                    createStepper_hw(
                            FULL_STEP_DOUBLE_PHASE,
                            &PORTD,
                            PD0, // STEP pin
                            PD1, // DIR pin
                            PD2, // ms1 pin
                            PD3, // ms2 pin
                            PD4  // ms3 pin
                    )
            ),
            0.0001
    );
    AxisState yAxis = initializeAxis(
            initiateStepper(
                    hardware_polulu,
                    createStepper_hw(
                            FULL_STEP_DOUBLE_PHASE,
                            &PORTD,
                            PD5, // STEP pin
                            PD6, // DIR pin
                            PD2, // ms1 pin
                            PD3, // ms2 pin
                            PD4  // ms3 pin
                    )
            ),
            0.0001
    );
    AxisState zAxis = initializeAxis(
             // TODO somehow this should be servo instead of stepper
            initiateStepper(
                    hardware_polulu,
                    createStepper_hw(
                            FULL_STEP_DOUBLE_PHASE,
                            PORTD,
                            PD7, // STEP pin
                            PD7, // DIR pin
                            PD2, // ms1 pin
                            PD3, // ms2 pin
                            PD4  // ms3 pin
                    )
            ),
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

AxisState initializeAxis(StepperState stepState, float stepSize) {
    return {
            .pos = 0.0,
            .stepState = stepState,
            .stepSize = stepSize,
    };
}