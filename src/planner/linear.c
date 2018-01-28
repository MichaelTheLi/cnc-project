//
// Created by Michael Lipinski on 28/01/2018.
//

#include <math.h>
#include <stdbool.h>
#include "linear.h"

/**
 * Should be sufficient for XY drawing plotter purposes
 */
enum PlannerResult bresenham_line_2d(Point from, Point to, Plan *output) {
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
        bool xSucceed = (to.x - from.x <= 0 && x_dir == plan_item_dir_forward)
                                 || (from.x - to.x <= 0 && x_dir == plan_item_dir_backward);
        bool ySucceed = (to.y - from.y <= 0 && y_dir == plan_item_dir_forward)
                        || (from.y - to.y <= 0 && y_dir == plan_item_dir_backward);
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
            };;
        }

        if (plan_i >= PLAN_SIZE) {
            return planner_full;
        }
    }

    return planner_success;
}