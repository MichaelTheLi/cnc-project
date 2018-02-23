/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <util/atomic.h>
#include "bits.h"
#include "delay.h"
#include "core/core.h"
#include "gcode/executor.h"
#include "serial/uart.h"
#include "serial/atmega32_adapter.h"

volatile unsigned char command_in[BUFFER_SIZE];

void ping(unsigned char n) {
    for (unsigned char i = 0; i < n; ++i) {
        bit_set(PORTA, BIT(0));
        delay_ms(100);
        bit_clear(PORTA, BIT(0));
        delay_ms(150);
    }
}

CNCPosition *cncPositionPtr;

void copy_command (char* data_in)
{
    strncpy((char *) command_in, (const char *) data_in, BUFFER_SIZE);
}

void process_command()
{
    char *commandStr = (char *) command_in;

    GCodeCommand *commandPtr = createCommand();
    GCodeParseResult parseResult = parseString(commandStr, commandPtr);

    if (parseResult == gcode_parse_error) {
        serial_put_string("-- GCode parse error\r\n");
        return;
    }

    if (parseResult == gcode_parse_success) {
        GCodeExecuteResult executeResult = executeCommand(commandPtr, cncPositionPtr);

        if (executeResult == gcode_execute_success) {
            serial_put_string("- Executed\r\n");
        } else {
            serial_put_string("-- Execute error\r\n");
        }
    }
}


int main(void)
{
    bit_set(DDRA, BIT(0));
    bit_set(DDRA, BIT(1));
    bit_set(DDRA, BIT(2));
    bit_set(DDRA, BIT(3));
    bit_set(DDRA, BIT(4));

    cncPositionPtr = initializeCNCPosition();

    serial_initialize(&copy_command, &isReady, &sendChar);
    initialize(64, &serial_onReceivedChar);

    delay_ms(100);

    serial_put_string("Initialized\r\n");
    delay_ms(1000);

    // !_INTERRUPTS_!
    sei();

    for(;;){
        delay_ms(100);
        ping(1);
        delay_ms(200);
        ping(1);

        if (command_in != NULL) {
            serial_put_string("--- Received\r\n");

            process_command();

            serial_put_string("ok\r\n");

            free(command_in);
        }
    }
}