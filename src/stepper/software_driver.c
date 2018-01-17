//
// Created by Michael Lipinski on 14/01/2018.
//

#include <stddef.h>
#include "software_driver.h"

StepState stepperStates[MAX_STEPPERS];

unsigned char stepsFullSingle[] = { // TODO same as double for now
    0b0110,
    0b0101,
    0b1001,
    0b1010,
};

unsigned char stepsFullDouble[] = {
    0b0110,
    0b0101,
    0b1001,
    0b1010,
};

unsigned char stepsHalf[] = {
    0b0110,
    0b0100,
    0b0101,
    0b0001,
    0b1001,
    0b1000,
    0b1010,
    0b0010,
};

void attachStepper(unsigned char stepperId, enum StepperMode mode) {
    if (stepperId >= MAX_STEPPERS) {
        return;
    }

     StepState state = {
        mode,
        0,
        0UL,
        CW
    };

    switch (mode) {
        case FULL_STEP_DOUBLE_PHASE:
            state.phasesCount = 4;
            state.phases = stepsFullSingle;
            break;
        case FULL_STEP_SINGLE_PHASE:
            state.phasesCount = 4;
            state.phases = stepsFullDouble;
            break;
        case HALF_STEP:
            state.phasesCount = 8;
            state.phases = stepsHalf;
            break;
    }

    state.attachMode = ATTACHED;

    stepperStates[stepperId] = state;
}

StepState getStepperState(unsigned char stepperId) {
    if (stepperId >= MAX_STEPPERS) {
        StepState defaultState = {
            FULL_STEP_SINGLE_PHASE,
            0,
            0UL,
            CW,
            4,
            {0},
            DETACHED
        };
        return defaultState;
    }

    return stepperStates[stepperId];
}

unsigned char getPinsValues(unsigned char stepperId) {
    StepState state = getStepperState(stepperId);

    return state.phases[state.phase];
}

void makeStep(unsigned char stepperId) {
    StepState state = getStepperState(stepperId);

    int phase = state.phase;

    if (state.direction == CW) {
        phase++;
    } else {
        phase--;
    }

    if (phase == state.phasesCount && state.direction == CW) {
        phase = 0;
    } else if (phase == -1 && state.direction == CCW) {
        phase = state.phasesCount - 1;
    }

    state.phase = (unsigned int) phase;
    stepperStates[stepperId] = state; // TODO Is it necessary?
}

void setDirection(unsigned char stepperId, enum StepperDirection direction) {
    stepperStates[stepperId].direction = direction;
}
