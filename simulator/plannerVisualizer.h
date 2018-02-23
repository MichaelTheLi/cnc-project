//
// Created by Michael Lipinski on 01/02/2018.
//

#ifndef CLION_AVR_PLANNERVISUALIZER_H
#define CLION_AVR_PLANNERVISUALIZER_H

#include <planner/linear.h>

void addRealLine(Point *start, Point *end);
void addPlanToRender(Plan *output, Point *pos, float render_step_size, char renderType);
int startPlannerVisualization();

#endif //CLION_AVR_PLANNERVISUALIZER_H
