//
// Created by Michael Lipinski on 22/01/2018.
//

#ifndef CLION_AVR_CORE_H
#define CLION_AVR_CORE_H

#include "../stepper/stepper.h"

typedef struct {
    StepperState stepState;
    float pos;
    float stepSize;
} AxisState;

typedef struct {
    AxisState x;
    AxisState y;
    AxisState z;
    float feedRate;
} CNCPosition;

CNCPosition* initializeCNCPosition();

#endif //CLION_AVR_CORE_H
