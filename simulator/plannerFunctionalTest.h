//
// Created by Michael Lipinski on 03/02/2018.
//

#ifndef CLION_AVR_PLANNERFUNCTIONALTEST_H
#define CLION_AVR_PLANNERFUNCTIONALTEST_H

#include "plannerVisualizer.h"

#include "planner/linear.h"
#include "planner/arc.h"

void testLinearInterpolation();
void testArcInterpolation();
void testArcCWInterpolation();
void testRealCommand(int size, char **commands);

#endif //CLION_AVR_PLANNERFUNCTIONALTEST_H
