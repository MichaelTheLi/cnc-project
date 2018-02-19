//
// Created by Michael Lipinski on 13/02/2018.
//

#include <avr/io.h>
#include "./driver.h"
#include "./pwm_calculator.h"

#define PRESCALER 64

void initializeATMegaHardwarePWM(ServoInnerState innerState) {
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);        // NON Inverted PWM
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10); //PRESCALER=64, FAST PWM mode

    // 311 for fPWM=50Hz (Period = 20ms Standard)
    ICR1 = (int) TOP_FOR_FREQ(50, PRESCALER);
}

void turn(ServoInnerState innerState) {
    float angle = innerState.currentAngle;

    OCR1A = (unsigned int) calculateValueForAngle(angle, 1500, 2000, PRESCALER);
}

