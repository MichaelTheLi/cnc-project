//
// Created by Michael Lipinski on 13/02/2018.
//

#include "pwm_calculator.h"

inline float calculatePeriodInUs(int prescaler);

float calculatePeriodInUs(int prescaler) {
    return ((float) prescaler / F_CPU) * 1000000;
}

float calculateValueForAngle(float angle, int timeFor0InUs, int timeFor90InUs, int prescaler) {
    const float valForZero = calculateValueForZero(timeFor0InUs, prescaler);
    const float perDegree = calculateIncrementPerDegree(timeFor0InUs, timeFor90InUs, prescaler);

    return (unsigned int) (valForZero + angle * perDegree);
}

float calculateValueForZero(int timeFor0InUs, int prescaler) {
    const float periodInUs = calculatePeriodInUs(prescaler);
    return timeFor0InUs / periodInUs;
}

float calculateIncrementPerDegree(int timeFor0InUs, int timeFor90InUs, int prescaler) {
    const float periodInUs = calculatePeriodInUs(prescaler);
    return ((float)(timeFor90InUs - timeFor0InUs) / 90) / periodInUs;
}
