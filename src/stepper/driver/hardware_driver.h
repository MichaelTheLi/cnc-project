//
// Created by Michael Lipinski on 25/01/2018.
//

#ifndef CLION_AVR_HARDWARE_DRIVER_H
#define CLION_AVR_HARDWARE_DRIVER_H

#include <stdint.h>
#include "../enums.h"
#include "../../bits.h"

typedef struct {
    enum StepperMode mode;
    enum StepperDirection dir;
    volatile uint8_t *port;
    unsigned char step_pin;
    unsigned char dir_pin;
    unsigned char ms1pin;
    unsigned char ms2pin;
    unsigned char ms3pin;
} StepState_hw_popolu_A4988;

StepState_hw_popolu_A4988 createStepper_hw(
    enum StepperMode mode,
    volatile uint8_t *port,
    unsigned char step_pin,
    unsigned char dir_pin,
    unsigned char ms1pin,
    unsigned char ms2pin,
    unsigned char ms3pin
);

void makeStepByPtr_hw(StepState_hw_popolu_A4988 *statePtr);
void setDirectionByPtr_hw(StepState_hw_popolu_A4988 *statePtr, enum StepperDirection direction);
void setModeByPtr_hw(StepState_hw_popolu_A4988 *statePtr, enum StepperMode mode);

#endif //CLION_AVR_HARDWARE_DRIVER_H
