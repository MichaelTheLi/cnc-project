//
// Created by Michael Lipinski on 25/01/2018.
//

#include "hardware_driver.h"
#include "../../delay.h"

StepState_hw_popolu_A4988 createStepper_hw(enum StepperMode mode,
                                           uint8_t *port,
                                           unsigned char step_pin,
                                           unsigned char dir_pin,
                                           unsigned char ms1pin,
                                           unsigned char ms2pin,
                                           unsigned char ms3pin
) {
    StepState_hw_popolu_A4988 state = {
        .mode = mode,
        .dir = CW,
        .port = port,
        .step_pin = step_pin,
        .dir_pin = dir_pin,
        .ms1pin = ms1pin,
        .ms2pin = ms2pin,
        .ms3pin = ms3pin,
    };

    return state;
}

void setMicrosteppingSettings(StepState_hw_popolu_A4988 *statePtr);

void makeByPtr_hw(StepState_hw_popolu_A4988 *statePtr) {
    uint8_t port = *statePtr->port;

    // Set microstepping settings
    setMicrosteppingSettings(statePtr);

    // Set direction
    if (statePtr->dir == CW) {
        bit_set(port, BIT(statePtr->dir_pin));
    } else {
        bit_clear(port, BIT(statePtr->dir_pin));
    }

    // make actual step pulse
    bit_set(port, BIT(statePtr->step_pin));
    delay_us(500);
    bit_clear(port, BIT(statePtr->step_pin));
    delay_us(500);
}

void setDirectionByPtr_hw(StepState_hw_popolu_A4988 *statePtr, enum StepperDirection direction) {
    statePtr->dir = direction;
}

void setMicrosteppingSettings(StepState_hw_popolu_A4988 *statePtr) {
    uint8_t port = *statePtr->port;

    switch (statePtr->mode) {
        case FULL_STEP_DOUBLE_PHASE:
        case FULL_STEP_SINGLE_PHASE:
            bit_clear(port, BIT(statePtr->ms1pin));
            bit_clear(port, BIT(statePtr->ms2pin));
            bit_clear(port, BIT(statePtr->ms3pin));
            break;
        case HALF_STEP:
            bit_set(port, BIT(statePtr->ms1pin));
            bit_clear(port, BIT(statePtr->ms2pin));
            bit_clear(port, BIT(statePtr->ms3pin));
            break;
        case QUARTER_STEP:
            bit_clear(port, BIT(statePtr->ms1pin));
            bit_set(port, BIT(statePtr->ms2pin));
            bit_clear(port, BIT(statePtr->ms3pin));
            break;
        case EIGHTH_STEP:
            bit_set(port, BIT(statePtr->ms1pin));
            bit_set(port, BIT(statePtr->ms2pin));
            bit_clear(port, BIT(statePtr->ms3pin));
            break;
        case SIXTEENTH_STEP:
            bit_set(port, BIT(statePtr->ms1pin));
            bit_set(port, BIT(statePtr->ms2pin));
            bit_set(port, BIT(statePtr->ms3pin));
            break;
    }
}