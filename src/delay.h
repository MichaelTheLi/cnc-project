//
// Created by Michael Lipinski on 25/01/2018.
//

#ifndef CLION_AVR_DELAY_H
#define CLION_AVR_DELAY_H

#ifdef DELAY_ENABLED
#include <util/delay.h>
#endif

#ifdef DELAY_ENABLED
#define delay_us(t) _delay_us(t)
#else
#define delay_us(t)
#endif

#endif //CLION_AVR_DELAY_H
