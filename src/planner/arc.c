//
// Created by Michael Lipinski on 02/02/2018.
//

#include <math.h>
#include "arc.h"

#define pow2(X) ((X) * (X))
#define error(X,Y,R) (fabs(pow2(X) + pow2(Y) - pow2(R)))

void nextStep(Point *pointPtr, float radius, Plan *output, unsigned int *plan_i, enum ArcDirection dir);
void nextStepCW(Point *pointPtr, float radius, Plan *output, unsigned int *plan_i, enum ArcDirection dir);
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
    double q = sqrt(pow2(two.x-one.x) + pow2(two.y - one.y));

    double y3 = (one.y+two.y)/2;
    double x3 = (one.x+two.x)/2;

    double basex = sqrt(pow2(radius)-pow2(q/2))*(one.y-two.y)/q; //calculate once
    double basey = sqrt(pow2(radius)-pow2(q/2))*(two.x-one.x)/q; //calculate once

    if (radius > 0) {
        return (Point) {
          .x = x3 + basex,
          .y = y3 + basey,
        };
    }

    return (Point) {
        .x = x3 - basex,
        .y = y3 - basey,
    };
}

float findAndCheckRadius(Point one, Point two, Point center, float tolerance) {
    double first = sqrt(pow2(center.x - one.x) + pow2(center.y - one.y));
    double second = sqrt(pow2(center.x - two.x) + pow2(center.y - two.y));

    float diff = fabs(first - second);
    if (diff >= tolerance * 100 / diff) {
        return NAN;
    }

    return first;
}

enum PlannerResult bresenham_arc_2d(Point from, Point to, float radius, Plan *output, Point *lastPoint, enum ArcDirection dir) {
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
        if (dir == ARC_CCW) {
            nextStep(&point, radius, output, &plan_i, dir);
        } else {
            nextStepCW(&point, radius, output, &plan_i, dir);
        }
    }

    *lastPoint = (Point) {
            .x = point.x,
            .y = point.y,
    };

    return planner_success;
}

PlanItem createXMoveSimple(int inc) {
    return (PlanItem) {
            .type = x_move,
            .direction = inc > 0 ? plan_item_dir_forward : plan_item_dir_backward,
    };
}

PlanItem createYMoveSimple(int inc) {
    return (PlanItem) {
            .type = y_move,
            .direction = inc > 0 ? plan_item_dir_forward : plan_item_dir_backward,
    };
}

PlanItem createXMove(int incX, int incY, enum ArcDirection dir) {
    int inc = dir == ARC_CCW ? incX : incY;
    return (PlanItem) {
            .type = dir == ARC_CCW ? x_move : y_move,
            .direction = dir == ARC_CCW
                 ? inc > 0 ? plan_item_dir_forward : plan_item_dir_backward
                 : inc > 0 ? plan_item_dir_backward : plan_item_dir_forward
    };
}

PlanItem createYMove(int incX, int incY, enum ArcDirection dir) {
    int inc = dir == ARC_CCW ? incY : incX;
    return (PlanItem) {
            .type = dir == ARC_CCW ? y_move : x_move,
            .direction = dir == ARC_CCW
                 ? inc > 0 ? plan_item_dir_forward : plan_item_dir_backward
                 : inc > 0 ? plan_item_dir_backward : plan_item_dir_forward
    };
}

void nextStepCW(Point *pointPtr, float radius, Plan *output, unsigned int *plan_i, enum ArcDirection dir) {
    Point point = *pointPtr;
    int incX;
    if (point.y > 0) {
        incX = 1;
    } else {
        incX = -1;
    }

    int incY;
    if (point.x > 0) {
        incY = -1;
    } else {
        incY = 1;
    }

    if (fabs(point.x) > fabs(point.y)) {
        float nextXError = error(point.x + incX, point.y + incY, radius);
        float XError = error(point.x, point.y + incY, radius);
        if (nextXError < XError) {
            point.x += incX;
            output->items[*plan_i] = createXMoveSimple(incX);
            (*plan_i)++;
        }

        point.y += incY;

        output->items[*plan_i] = createYMoveSimple(incY);
        (*plan_i)++;
    }
    if (fabs(point.x) <= fabs(point.y)) {
        float nextYError = error(point.x + incX, point.y + incY, radius);
        float YError = error(point.x + incX, point.y, radius);
        if (nextYError < YError) {
            point.y += incY;
            output->items[*plan_i] = createYMoveSimple(incY);
            (*plan_i)++;
        }
        point.x += incX;

        output->items[*plan_i] = createXMoveSimple(incX);
        (*plan_i)++;
    }
    *pointPtr = point;
}

void nextStep(Point *pointPtr, float radius, Plan *output, unsigned int *plan_i, enum ArcDirection dir) {
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
            output->items[*plan_i] = createXMove(incX, incY, dir);
            (*plan_i)++;
        }

        point.y += incY;

        output->items[*plan_i] = createYMove(incX, incY, dir);
        (*plan_i)++;
    }
    if (fabs(point.x) <= fabs(point.y)) {
        float nextYError = error(point.x + incX, point.y + incY, radius);
        float YError = error(point.x + incX, point.y, radius);
        if (nextYError < YError) {
            point.y += incY;
            output->items[*plan_i] = createYMove(incX, incY, dir);
            (*plan_i)++;
        }
        point.x += incX;

        output->items[*plan_i] = createXMove(incX, incY, dir);;
        (*plan_i)++;
    }
    *pointPtr = point;
}

unsigned char complete(Point point, Point to) {
    if(check(point, to)) {
        float tolerance = 0;

        if ((fabs(point.y) - fabs(to.y)) + (fabs(point.x) - fabs(to.x)) < 4) {
            tolerance = 2;
        }

        if(fabs(point.x) > fabs(point.y)) {
            if(fabs(point.y - to.y) <= tolerance) {
                return 1;
            }
        } else {
            if(fabs(point.x - to.x) <= tolerance) {
                return 1;
            }
        }
    }

    return 0;
}

unsigned char check(Point point, Point to) {
    if((point.x > 0 && to.x > 0) || (point.x < 0 && to.x < 0) || (point.x == 0 && to.x == 0)) {
        if ((point.y > 0 && to.y > 0) || (point.y < 0 && to.y < 0) || (point.y == 0 && to.y == 0)) {
            if (((fabs(point.x) >= fabs(point.y)) && (fabs(to.x) >= fabs(to.y)))
                || ((fabs(point.x) < fabs(point.y)) && (fabs(to.x) < fabs(to.y)))
            ) {
                return 1;
            }
        }
    }

    return 0;
}