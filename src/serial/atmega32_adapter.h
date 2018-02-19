//
// Created by Michael Lipinski on 19/02/2018.
//

#ifndef CLION_AVR_ATMEGA32_ADAPTER_H
#define CLION_AVR_ATMEGA32_ADAPTER_H

#include <stdbool.h>

typedef void (*receivedCharFunc)(unsigned int receivedChar);

void initialize(unsigned char prescale, receivedCharFunc receivedChar_callback);
bool isReady();
void sendChar(char toSend);

#endif //CLION_AVR_ATMEGA32_ADAPTER_H
