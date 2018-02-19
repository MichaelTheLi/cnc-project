//
// Created by Michael Lipinski on 31/01/2018.
//

#ifndef CLION_AVR_UART_H
#define CLION_AVR_UART_H

#include <stdbool.h>

#define BUFFER_SIZE 64

typedef void (*commandReady_func)(char*);
typedef bool (*isReady_func)();
typedef void (*sendChar_func)(char toSend);

void serial_initialize(commandReady_func command_ready,
                       isReady_func isReadyFunc,
                       sendChar_func sendCharFunc);

void serial_put_char(char send);
void serial_put_string(const char *send);

void serial_onReceivedChar(char receivedChar);

#endif //CLION_AVR_UART_H
