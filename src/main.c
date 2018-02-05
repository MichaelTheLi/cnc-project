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

void ping(unsigned char n) {
    for (unsigned char i = 0; i < n; ++i) {
        bit_set(PORTA, BIT(0));
        delay_ms(100);
        bit_clear(PORTA, BIT(0));
        delay_ms(150);
    }
}

#define FOSC 1000000UL
#define BAUD 3600
#define BAUD_PRESCALE ((FOSC/16/BAUD) - 1)

#define TRUE 1
#define FALSE 0

#define CHAR_NEWLINE '\n'
#define CHAR_RETURN '\r'
#define RETURN_NEWLINE "\r\n"

// The inputted commands are never going to be
// more than 8 chars long. Volatile for the ISR.
#define BUFFER_SIZE 16
volatile unsigned char data_in[BUFFER_SIZE];
volatile unsigned char command_in[BUFFER_SIZE];

volatile unsigned char data_count = 0;
volatile unsigned char command_ready;


void usart_putc (char send)
{
    // Do nothing for a bit if there is already
    // data waiting in the hardware to be sent
    while ( !( UCSRA & (1<<UDRE)) );

    UDR = send;
}

void usart_puts (const char *send)
{
    // Cycle through each character individually
    while (*send) {
        usart_putc(*send++);
    }
}

void copy_command ()
{
    // The USART might interrupt this - don't let that happen!
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        strncpy(command_in, data_in, BUFFER_SIZE); // -2 because of end of line
        memset(&data_in[0], 0, BUFFER_SIZE);
    }
}
void process_command()
{
    char *commandStr = (char *) command_in;

    if (strcmp(commandStr, "on\r\n")== 0) {
        usart_puts("process_command_on\r\n");
        bit_set(PORTA, BIT(0));
    }

    if (strcmp(commandStr, "off\r\n") == 0){
        usart_puts("process_command_off\r\n");
        bit_clear(PORTA, BIT(0));
    }
}


ISR (USART_RXC_vect)
{
    // Get data from the USART in register
    data_in[data_count] = UDR;

    usart_putc(data_in[data_count]);

    // End of line!
    if (data_in[data_count] == '\n') {
        command_ready = TRUE;
        // Reset to 0, ready to go again
        data_count = 0;
    } else {
        data_count++;
    }
}

int main(void)
{
    bit_set(DDRA, BIT(0));

    // Set baud rate
    UBRRL = BAUD_PRESCALE;
    UBRRH = (BAUD_PRESCALE >> 8);

    // Turn on USART hardware (RX, TX)
    UCSRB |= (1 << RXEN) | (1 << TXEN);

    UCSRC = (1<<URSEL)|(1<<USBS)|(1<<UCSZ0)|(1<<UCSZ1); // 8 bit, 2 stops
    UCSRC |= (1<<UPM1) | (1<<UPM0); //Odd parity
    // Enable the USART Receive interrupt
    UCSRB |= (1 << RXCIE );

    usart_puts("Initialized\r\n");

    delay_ms(1000);
    // Globally enable interrupts
    sei();

    ping(1);
    _delay_ms(1000);
    ping(2);
    _delay_ms(500);
    ping(1);
    delay_ms(1000);
    ping(2);
    delay_ms(500);

    for(;;){
        if (command_ready == TRUE) {
            copy_command();
            process_command();

            usart_puts("ok\r\n");

            command_ready = FALSE;
        }
    }
}