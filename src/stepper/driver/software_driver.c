//
// Created by Michael Lipinski on 14/01/2018.
//

#include <stddef.h>
#include <stdint.h>
#include "software_driver.h"
#include "../../bits.h"
#include "../../core/core.h"

StepState stepperStates[MAX_SOFTWARE_STEPPERS];

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

unsigned char getPinsValuesByPtr(StepState *statePtr);
void setPortOutput(StepState *statePtr);

StepState createStepper(enum StepperMode mode, uint8_t *port, ConnectedPins pins) {
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
    state.port = port;
    state.pins = pins;

    setPortOutput(&state);

    return state;
}

StepState* attachStepper(unsigned char stepperId, enum StepperMode mode, uint8_t *port, ConnectedPins pins) {
    if (stepperId >= MAX_SOFTWARE_STEPPERS) {
        return NULL;
    }

    stepperStates[stepperId] = createStepper(mode, port, pins);

    return &stepperStates[stepperId];
}

StepState* getStepperState(unsigned char stepperId) {
    if (stepperId >= MAX_SOFTWARE_STEPPERS) {
        return NULL;
    }

    return &stepperStates[stepperId];
}

void makeStep(unsigned char stepperId) {
    StepState* statePtr = getStepperState(stepperId);

    makeStepByPtr(statePtr);
}

void setDirection(unsigned char stepperId, enum StepperDirection direction) {
    setDirectionByPtr(&stepperStates[stepperId], direction);
}

void makeStepByPtr(StepState *statePtr) {
    if (!statePtr) {
        return;
    }

    StepState state = *statePtr;

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
    *statePtr = state;

    // Output to hardware
    setPortOutput(statePtr);
}

void setPortOutput(StepState *statePtr) {
    unsigned char pins = getPinsValuesByPtr(statePtr);
    uint8_t port = *statePtr->port;
    bit_write(bit_get(pins, BIT(0)), port, BIT(statePtr->pins.A1_pin));
    bit_write(bit_get(pins, BIT(1)), port, BIT(statePtr->pins.A2_pin));
    bit_write(bit_get(pins, BIT(2)), port, BIT(statePtr->pins.B1_pin));
    bit_write(bit_get(pins, BIT(3)), port, BIT(statePtr->pins.B2_pin));

    *statePtr->port = port;
}

unsigned char getPinsValuesByPtr(StepState *statePtr) {
    if (!statePtr) {
        return NULL;
    }

    StepState state = *statePtr;

    return state.phases[state.phase];
}

void setDirectionByPtr(StepState *statePtr, enum StepperDirection direction) {
    statePtr->direction = direction;
}
