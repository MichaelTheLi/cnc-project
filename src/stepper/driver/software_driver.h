//
// Created by Michael Lipinski on 14/01/2018.
//

#ifndef CLION_AVR_SOFTWARE_DRIVER_H
#define CLION_AVR_SOFTWARE_DRIVER_H

#include <stdint.h>
#include "../enums.h"

#define MAX_SOFTWARE_STEPPERS 10 // Just because

enum StepperAttachMode { ATTACHED, DETACHED };

typedef struct {
    unsigned char A1_pin;
    unsigned char A2_pin;
    unsigned char B1_pin;
    unsigned char B2_pin;
} ConnectedPins;

typedef struct {
    enum StepperMode mode;
    unsigned int phase;
    unsigned long step;
    enum StepperDirection direction;
    unsigned char phasesCount;
    unsigned char *phases;
    enum StepperAttachMode attachMode;
    uint8_t *port;
    ConnectedPins pins;
} StepState;

StepState createStepper(enum StepperMode mode, uint8_t *port, ConnectedPins pins);
StepState* attachStepper(unsigned char stepperId, enum StepperMode mode, uint8_t *port, ConnectedPins pins);
StepState* getStepperState(unsigned char stepperId);

void makeStep(unsigned char stepperId);
void setDirection(unsigned char stepperId, enum StepperDirection direction);

void makeStepByPtr(StepState *stepState);
void setDirectionByPtr(StepState *stepState, enum StepperDirection direction);

#endif //CLION_AVR_SOFTWARE_DRIVER_H
