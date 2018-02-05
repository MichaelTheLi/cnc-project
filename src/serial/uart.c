////
//// Created by Michael Lipinski on 31/01/2018.
////
//
//#include "uart.h"
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <util/atomic.h>
//
//#include "../bits.h"
//
//void USART_init()
//{
//    /*Set baud rate */
//    ATOMIC_BLOCK(ATOMIC_FORCEON) {
//        UBRRH = (MYUBRR >> 8);
//        UBRRL = MYUBRR;
//    }
//
//    bit_set(UCSRB, BIT(RXEN));  // Enable receiver
//    bit_set(UCSRB, BIT(TXEN));  // Enable transmitter
//    bit_set(UCSRB, BIT(RXCIE)); // Enable the receiver interrupt
//
//    bit_set(UCSRC, BIT(UCSZ0)); // 8bit
//    bit_set(UCSRC, BIT(UCSZ1)); //
//    bit_set(UCSRC, BIT(URSEL)); // Register select. High because
//}
//
//void USART_Transmit( char data )
//{
//    /* Wait for empty transmit buffer */
//    while ( !( UCSRA & (1<<UDRE)) );
//    /* Put data into buffer, sends the data */
//    UDR = data;
//}
//
//
//
//ISR (USART_RXC_vect)
//{
////    command = UDR;
//}