//
// Created by Michael Lipinski on 13/02/2018.
//

#include "driver.h"

void goToAngle(ServoState *state, float angle) {
    state->innerState.currentAngle = angle;

    state->turner(state->innerState);
}

void initializeServo(ServoState *state) {
    state->initializer(state->innerState);
}

ServoState composeServoState(void (*initializer)(ServoInnerState), void (*turner)(ServoInnerState)) {
    ServoInnerState innerState = {
        .currentAngle = 0
    };

    return (ServoState){
        .innerState = innerState,
        .initializer = initializer,
        .turner = turner,
    };
}
