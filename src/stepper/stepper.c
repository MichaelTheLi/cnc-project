//
// Created by Michael Lipinski on 25/01/2018.
//

#include <avr/io.h>
#include "stepper.h"

StepperState initiateStepper(enum StepperDriverType type, InnerStepperState state) {
    return {
        .type = type,
        .state = state,
    };
}

void dir(StepperState *state, enum StepperDirection dir) {
    if (state->type == software) {
        setDirectionByPtr(&state->state.s_state, dir);
    } else {
        setDirectionByPtr_hw(&state->state.h_state, dir);
    }
}

void step(StepperState *state) {
    if (state->type == software) {
        StepState *ptr = &state->state.s_state;
        makeStepByPtr(ptr);

    } else {
        StepState_hw_popolu_A4988 *ptr = &state->state.h_state;
        makeStepByPtr_hw(ptr);
    }
}