//
// Created by Michael Lipinski on 14/01/2018.
//

#ifndef CLION_AVR_SOFTWARE_DRIVER_H
#define CLION_AVR_SOFTWARE_DRIVER_H

#define MAX_STEPPERS 10 // Just because

enum StepperMode { FULL_STEP_SINGLE_PHASE, FULL_STEP_DOUBLE_PHASE, HALF_STEP };
enum StepperDirection { CW, CCW };
enum StepperAttachMode { ATTACHED, DETACHED };

typedef struct {
    enum StepperMode mode;
    unsigned int phase;
    unsigned long step;
    enum StepperDirection direction;
    unsigned char phasesCount;
    unsigned char *phases;
    enum StepperAttachMode attachMode;
} StepState;


void attachStepper(unsigned char stepperId, enum StepperMode mode);
StepState getStepperState(unsigned char stepperId);

void makeStep(unsigned char stepperId);
void setDirection(unsigned char stepperId, enum StepperDirection direction);
unsigned char getPinsValues(unsigned char stepperId);

#endif //CLION_AVR_SOFTWARE_DRIVER_H
