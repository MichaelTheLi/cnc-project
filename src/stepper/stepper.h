//
// Created by Michael Lipinski on 25/01/2018.
//

#ifndef CLION_AVR_STEPPER_H
#define CLION_AVR_STEPPER_H

#include "driver/software_driver.h"
#include "driver/hardware_driver.h"

enum StepperDriverType { software, hardware_polulu };

typedef union {
    StepState s_state;
    StepState_hw_popolu_A4988 h_state;
} InnerStepperState;

typedef struct {
    enum StepperDriverType type;
    InnerStepperState state;
} StepperState;

#endif //CLION_AVR_STEPPER_H
