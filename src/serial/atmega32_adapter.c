//
// Created by Michael Lipinski on 19/02/2018.
//

#include "atmega32_adapter.h"

#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>

volatile receivedCharFunc receivedCharCallback;

void initialize(unsigned char prescale, receivedCharFunc receivedChar_callback) {
    receivedCharCallback = receivedChar_callback;

    // Set baud rate
    UBRRL = prescale;
    UBRRH = (prescale >> 8);

    // Turn on USART hardware (RX, TX)
    UCSRB |= (1 << RXEN) | (1 << TXEN);

    UCSRC = (1 << URSEL) | (1 << USBS) | (1 << UCSZ0) | (1 << UCSZ1); // 8 bit, 2 stops
    UCSRC |= (1 << UPM1) | (1 << UPM0); // Odd parity

    // Enable the USART Receive interrupt
    UCSRB |= (1 << RXCIE);
}

bool isReady() {
    return (bool) (UCSRA & (1 << UDRE));
}

ISR (USART_RXC_vect)
{
    // Get data from the USART in register
    uint8_t receivedChar = UDR;
    receivedCharCallback((char) receivedChar);
}

void sendChar(char toSend) {
    UDR = (uint8_t) toSend;
}
