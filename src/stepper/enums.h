//
// Created by Michael Lipinski on 25/01/2018.
//

#ifndef CLION_AVR_ENUMS_H
#define CLION_AVR_ENUMS_H

enum StepperMode {
    FULL_STEP_SINGLE_PHASE,
    FULL_STEP_DOUBLE_PHASE,
    HALF_STEP,
    QUARTER_STEP,
    EIGHTH_STEP,
    SIXTEENTH_STEP,
};
enum StepperDirection { CW, CCW };

#endif //CLION_AVR_ENUMS_H
