//
// Created by Michael Lipinski on 02/02/2018.
//

#ifndef CLION_AVR_ARC_H
#define CLION_AVR_ARC_H

#include "planner.h"

void convert_coords_to_bresenham_arc_2d(Point *from, Point *to, float *radius, Point center, Point stepSizes);

Point convertPointToStepsSize(Point point, Point center, Point stepSizes);
Point convertPointFromStepsSize(Point point, Point center, Point stepSizes);
Point findCenter(Point one, Point two, float radius);

enum PlannerResult bresenham_arc_2d(Point from, Point to, float radius, Plan *output, Point *lastPoint);

#endif //CLION_AVR_ARC_H
