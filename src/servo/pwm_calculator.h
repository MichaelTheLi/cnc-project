//
// Created by Michael Lipinski on 13/02/2018.
//

#ifndef CLION_AVR_PWM_CALCULATOR_H
#define CLION_AVR_PWM_CALCULATOR_H

inline float calculateValueForAngle(float angle, int timeFor0InUs, int timeFor90InUs, int prescaler);
inline float calculateValueForZero(int timeFor0InUs, int prescaler);
inline float calculateIncrementPerDegree(int timeFor0InUs, int timeFor90InUs, int prescaler);

#endif //CLION_AVR_PWM_CALCULATOR_H
