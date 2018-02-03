//
// Created by Michael Lipinski on 28/01/2018.
//

#ifndef CLION_AVR_LINEAR_H
#define CLION_AVR_LINEAR_H

#include "planner.h"

void convert_coords_to_bresenham_line_2d(Point *from, Point *to, Point stepSizes);
Point convertPointToStepsSize_line(Point point, Point stepSizes);
Point convertPointFromStepsSize_line(Point point, Point stepSizes);

enum PlannerResult bresenham_line_2d(Point from, Point to, Plan *output, Point *lastPoint);

#endif //CLION_AVR_LINEAR_H
