//
// Created by Michael Lipinski on 13/02/2018.
//

#ifndef CLION_AVR_DRIVER_H
#define CLION_AVR_DRIVER_H


#include <stddef.h>
#include "../delay.h"


typedef struct {
    float currentAngle;
} ServoInnerState;
typedef struct {
    ServoInnerState innerState;
    void (*initializer)(ServoInnerState);
    void (*turner)(ServoInnerState);
} ServoState;


ServoState composeServoState(void (*initializer)(ServoInnerState), void (*turner)(ServoInnerState));
void initializeServo(ServoState *state);
void goToAngle(ServoState *state, float angle);


#endif //CLION_AVR_DRIVER_H
