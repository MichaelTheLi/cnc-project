//
// Created by Michael Lipinski on 22/01/2018.
//

#ifndef CLION_AVR_CORE_H
#define CLION_AVR_CORE_H

#include "../stepper/driver/software_driver.h"

typedef struct {
    StepState *stepState;
    float pos;
    float stepSize;
} AxisState;

typedef struct {
    AxisState x;
    AxisState y;
    AxisState z;
    float feedRate;
} CNCPosition;

#endif //CLION_AVR_CORE_H
