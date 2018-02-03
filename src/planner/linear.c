//
// Created by Michael Lipinski on 28/01/2018.
//

#include <math.h>
#include <stdbool.h>
#include "linear.h"

void convert_coords_to_bresenham_line_2d(Point *from, Point *to, Point stepSizes) {
    *from = convertPointToStepsSize_line(*from, stepSizes);
    *to = convertPointToStepsSize_line(*to, stepSizes);
}

Point convertPointToStepsSize_line(Point point, Point stepSizes) {
    point.x = floor(point.x / stepSizes.x);
    point.y = floor(point.y / stepSizes.y);

    return point;
}

Point convertPointFromStepsSize_line(Point point, Point stepSizes) {
    point.x = point.x * stepSizes.x;
    point.y = point.y * stepSizes.y;

    return point;
}


/**
 * Should be sufficient for XY drawing plotter purposes
 */
enum PlannerResult bresenham_line_2d(Point from, Point to, Plan *output, Point *lastPoint) {
    float dx_f = to.x - from.x;
    float dy_f = to.y - from.y;
    enum PlanItemDirection x_dir = dx_f > 0
                                   ? plan_item_dir_forward
                                   : plan_item_dir_backward;
    enum PlanItemDirection y_dir = dy_f > 0
                                   ? plan_item_dir_forward
                                   : plan_item_dir_backward;
    int dx = (int) fabs(dx_f);
    int sx = from.x < to.x ? 1 : -1;
    int dy = (int) fabs(dy_f);
    int sy = from.y < to.y ? 1 : -1;

    int err = (dx > dy ? dx : -dy) / 2, e2;

    int plan_i = 0;
    for (;;) {
        to.x = floor(to.x);
        to.y = floor(to.y);
        from.x = floor(from.x);
        from.y = floor(from.y);
        bool xSucceed = fabs(to.x - from.x) <= 0;
        bool ySucceed = fabs(to.y - from.y) <= 0;
        if (xSucceed && ySucceed) {
            break;
        }

        e2 = err;

        // TODO Contant order of moves can cause artifacts on line (?)
        if (e2 > -dx) {
            err -= dy;
            from.x += sx;
            output->items[plan_i++] = (PlanItem) {
                    .type = x_move,
                    .direction = x_dir
            };
        }
        if (e2 < dy) {
            err += dx;
            from.y += sy;
            output->items[plan_i++] = (PlanItem) {
                    .type = y_move,
                    .direction = y_dir
            };
        }

        if (plan_i >= PLAN_SIZE) {
            *lastPoint = (Point) {
                    .x = from.x,
                    .y = from.y,
            };
            return planner_full;
        }
    }

    return planner_success;
}