//
// Created by Michael Lipinski on 13/02/2018.
//

#ifndef CLION_AVR_ATMEGA_ADAPTER_H
#define CLION_AVR_ATMEGA_ADAPTER_H

#include "./driver.h"

void initializeATMegaHardwarePWM(ServoInnerState innerState);
void turn(ServoInnerState innerState);

#endif //CLION_AVR_ATMEGA_ADAPTER_H
