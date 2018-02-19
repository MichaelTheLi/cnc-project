//
// Created by Michael Lipinski on 31/01/2018.
//

#include <string.h>
#include <util/atomic.h>
#include "uart.h"

typedef struct {
    char data_in[BUFFER_SIZE];
    unsigned char data_count;

    isReady_func isReady;
    sendChar_func sendChar;

    commandReady_func commandReady;
} SerialState;

volatile SerialState serialState = {};

void serial_initialize(commandReady_func command_ready,
                       isReady_func isReadyFunc,
                       sendChar_func sendCharFunc) {
    serialState.commandReady = command_ready;
    serialState.isReady = isReadyFunc;
    serialState.sendChar = sendCharFunc;
}

void serial_put_char(char send)
{
    // Do nothing for a bit if there is already
    // data waiting in the hardware to be sent
    while (!serialState.isReady());

    serialState.sendChar(send);
}

void serial_put_string(const char *send)
{
    while (*send) {
        serial_put_char(*send++);
    }
}

void serial_onReceivedChar(char receivedChar) {
    serialState.data_in[serialState.data_count] = receivedChar;

    serial_put_char(receivedChar);

    if (receivedChar == '\n') {
        // The USART might interrupt this - don't let that happen!
        ATOMIC_BLOCK(ATOMIC_FORCEON)
        {
            serialState.commandReady((char *) serialState.data_in);
            memset((void *) &serialState.data_in[0], 0, BUFFER_SIZE);
        }
        serialState.data_count = 0;
    } else {
        serialState.data_count++;
    }
}
