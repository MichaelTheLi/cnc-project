//
// Created by Michael Lipinski on 02/02/2018.
//

#include <math.h>
#include "arc.h"

#define pow2(X) ((X) * (X))
#define error(X,Y,R) (fabs(pow2(X) + pow2(Y) - pow2(R)))

void nextStep(Point *pointPtr, float radius, Plan *output, unsigned int *plan_i);
unsigned char check(Point point, Point to);
unsigned char complete(Point point, Point to);

void convert_coords_to_bresenham_arc_2d(Point *from, Point *to, float *radius, Point center, Point stepSizes) {
    *from = convertPointToStepsSize(*from, center, stepSizes);
    *to = convertPointToStepsSize(*to, center, stepSizes);

    *radius = fabs(floor(*radius / stepSizes.x));
}

Point convertPointToStepsSize(Point point, Point center, Point stepSizes) {
    point.x = floor((point.x - center.x) / stepSizes.x);
    point.y = floor((point.y - center.y) / stepSizes.y);

    return point;
}

Point convertPointFromStepsSize(Point point, Point center, Point stepSizes) {
    point.x = (point.x * stepSizes.x) + center.x;
    point.y = (point.y * stepSizes.y) + center.y;

    return point;
}

Point findCenter(Point one, Point two, float radius) {
    float q = sqrt(pow2(two.x-one.x) + pow2(two.y - one.y));

    double y3 = (one.y+two.y)/2;
    double x3 = (one.x+two.x)/2;

    double basex = sqrt(pow2(radius)-pow2(q/2))*(one.y-two.y)/q; //calculate once
    double basey = sqrt(pow2(radius)-pow2(q/2))*(two.x-one.x)/q; //calculate once

    if (radius > 0) {
        return (Point) {
          .x = x3 + basex,
          .y =  y3 + basey,
        };
    }

    return (Point) {
        .x = x3 - basex,
        .y = y3 - basey,
    };
}

enum PlannerResult bresenham_arc_2d(Point from, Point to, float radius, Plan *output, Point *lastPoint) {
    Point point = from;

    unsigned int plan_i = 0;
    while(!complete(point, to)) {
        if (plan_i >= PLAN_SIZE - 1) {
            *lastPoint = (Point) {
                .x = point.x,
                .y = point.y,
            };
            return planner_full;
        }
        nextStep(&point, radius, output, &plan_i);
    }

    return planner_success;
}

void nextStep(Point *pointPtr, float radius, Plan *output, unsigned int *plan_i) {
    Point point = *pointPtr;
    int incX;
    if (point.y > 0) {
        incX = -1;
    } else {
        incX = 1;
    }

    int incY;
    if (point.x > 0) {
        incY = 1;
    } else {
        incY = -1;
    }

    if (fabs(point.x) > fabs(point.y)) {
        float nextXError = error(point.x + incX, point.y + incY, radius);
        float XError = error(point.x, point.y + incY, radius);
        if (nextXError < XError) {
            point.x += incX;
            output->items[*plan_i] = (PlanItem) {
                    .type = x_move,
                    .direction = incX > 0 ? plan_item_dir_forward : plan_item_dir_backward
            };
            (*plan_i)++;
        }

        point.y += incY;

        output->items[*plan_i] = (PlanItem) {
                .type = y_move,
                .direction = incY > 0 ? plan_item_dir_forward : plan_item_dir_backward
        };
        (*plan_i)++;
    }
    if (fabs(point.x) <= fabs(point.y)) {
        float nextYError = error(point.x + incX, point.y + incY, radius);
        float YError = error(point.x + incX, point.y, radius);
        if (nextYError < YError) {
            point.y += incY;
            output->items[*plan_i] = (PlanItem) {
                    .type = y_move,
                    .direction = incY > 0 ? plan_item_dir_forward : plan_item_dir_backward
            };
            (*plan_i)++;
        }
        point.x += incX;

        output->items[*plan_i] = (PlanItem) {
                .type = x_move,
                .direction = incX > 0 ? plan_item_dir_forward : plan_item_dir_backward
        };
        (*plan_i)++;
    }
    *pointPtr = point;
}

unsigned char complete(Point point, Point to) {
    if(check(point, to)) {
        if(fabs(point.x) > fabs(point.y)) {
            if(point.y == to.y) {
                return 1;
            }
        } else {
            if(point.x == to.x) {
                return 1;
            }
        }
    }

    return 0;
}

unsigned char check(Point point, Point to) {
    if((point.x>0 && to.x>0)||(point.x<0 && to.x<0)||(point.x==0 && to.x==0)) {
        if((point.y>0&&to.y>0)||(point.y<0&&to.y<0)||(point.y==0 && to.y==0)) {
            if(((fabs(point.x)>fabs(point.y))&&(fabs(to.x)>fabs(to.y)))|| ((fabs(point.x)<fabs(point.y))&&(fabs(to.x)<fabs(to.y)))) {
                return 1;
            }
        }
    }

    return 0;
}