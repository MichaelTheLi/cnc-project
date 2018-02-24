//
// Created by Michael Lipinski on 22/01/2018.
//

#include "executor.h"
#include "math.h"
#include "../planner/linear.h"
#include "../planner/arc.h"
#include "../core/core.h"

#ifdef TEST_EXECUTOR
    #include "../../simulator/plannerVisualizer.h"
#endif

void executeFastLinearMovement(float x, float y, float z, CNCPosition *cncPosition);
void executeLinearMovement(float x, float y, float z, CNCPosition *cncPosition);
void executeArcMovement(float x, float y, float radius, Point center, enum ArcDirection dir, CNCPosition *cncPosition);
void executePlan(Plan *plan, CNCPosition *cncPosition);
void executePlan_test(Plan *plan, enum PlannerResult result, Point lastPoint, CNCPosition *cncPosition, char render);

GCodeExecuteResult executeCommand(GCodeCommand *gCodeCommand, CNCPosition *cncPosition) {
    GCodeCommand command = *gCodeCommand;

    float x = command[COMMAND_INDEX('X')];
    float y = command[COMMAND_INDEX('Y')];
    float z = command[COMMAND_INDEX('Z')];

    int commandInt = (int) command[COMMAND_INDEX('G')];
    switch (commandInt) {
        case 0:
        case 1:
            if (isnan(x) && isnan(y) && isnan(z)) {
                return gcode_g0_not_enough_params;
            }

            if (commandInt == 0) {
                executeFastLinearMovement(x, y, z, cncPosition);
            } else {
                executeLinearMovement(x, y, z, cncPosition);
            }
            break;
        case 2:
        case 3: {
            float radius = command[COMMAND_INDEX('R')];
            Point from = {
                .x = cncPosition->x.pos,
                .y = cncPosition->y.pos
            };
            Point to = {
                .x = x,
                .y = y
            };
            enum ArcDirection dir = commandInt == 2
                ? ARC_CW
                : ARC_CCW;

            Point center;

            if (isnan(radius)) {
                float centerX = command[COMMAND_INDEX('I')];
                float centerY = command[COMMAND_INDEX('J')];

                center = (Point) {
                    .x = cncPosition->x.pos + centerX,
                    .y = cncPosition->y.pos + centerY
                };

                radius = findAndCheckRadius(from, to, center, cncPosition->x.stepSize);
            } else {
                center = findCenter(from, to, radius);
            }

            if (isnan(radius)) {
                return gcode_g2_g3_no_radius;
            }

            if (commandInt == 2) {
                radius *= -1;
            }

            executeArcMovement(x, y, radius, center, dir, cncPosition);
            break;
        }
        default:
            break;
    }

    return gcode_execute_success;
}

void executeActualArc(Point from, Point to, float radius, Point center, Point stepSizes, enum ArcDirection dir, CNCPosition *cncPosition) {
    enum PlannerResult result;
    Point lastPoint = from;
    do {
        Plan plan = {};
        result = bresenham_arc_2d(from, to, radius, &plan, &lastPoint, dir);

#ifndef TEST_EXECUTOR
        // TODO Should be executed somewhere else
        executePlan(&plan, cncPosition);
#else
        Point lastPointForRender = convertPointFromStepsSize(lastPoint, center, stepSizes);
        executePlan_test(&plan, result, lastPointForRender, cncPosition, (dir == ARC_CW) ? 2 : 3);
#endif
        if (result == planner_full) {
            from = lastPoint;
        }
    } while(result != planner_success && result != planner_fail);
}

void executeArcMovement(float x, float y, float radius, Point center, enum ArcDirection dir, CNCPosition *cncPosition) {
    Point from = {
            .x = cncPosition->x.pos,
            .y = cncPosition->y.pos
    };
    Point to = {
            .x = x,
            .y = y
    };

    Point stepSizes = {
            .x = cncPosition->x.stepSize,
            .y = cncPosition->y.stepSize,
    };

    convert_coords_to_bresenham_arc_2d(&from, &to, &radius, center, stepSizes);
    executeActualArc(from, to, radius, center, stepSizes, dir, cncPosition);

    // TODO Doing this we ignore to discrete movements error. Should be reported maybe? Should not accumulate for sure
    if (!isnan(x)) {
        cncPosition->x.pos = x;
    }
    if (!isnan(y)) {
        cncPosition->y.pos = y;
    }
}

void executeFastLinearMovement(float x, float y, float z, CNCPosition *cncPosition) {
    do {
        Point from = {
                .x = cncPosition->x.pos,
                .y = cncPosition->y.pos
        };
        Point to = {
                .x = x,
                .y = y
        };
        Point stepSizes = {
                .x = cncPosition->x.stepSize,
                .y = cncPosition->y.stepSize,
        };

        float dx = fabs(to.x - from.x);
        float dy = fabs(to.y - from.y);
        float dzRaw = z - cncPosition->z.pos;
        float dz = fabs(z - cncPosition->z.pos);

        int plan_i = 0;
        Plan plan = {};
        Point lastPoint = to;
        enum PlannerResult result = planner_fail;

        if ((dx || dy) && !isnan(dx) && !isnan(dy)) {
            convert_coords_to_bresenham_line_2d(&from, &to, stepSizes);

            result = bresenham_line_2d(from, to, &plan, &lastPoint);
        } else{
            if (dz) {
//            float virtZ = cncPosition->z.pos;
                // TODO Simple movement suitable only for XY-plotter
//            while(virtZ < z) {
//                plan.items[plan_i++] = (PlanItem) {
//                    .type = z_move,
//                    .direction = dzRaw > 0 ? plan_item_dir_forward : plan_item_dir_backward
//                };
//                int modifier = dzRaw > 0 ? 1 : -1;
//                virtZ += modifier * cncPosition->z.stepSize;
//            }
            }

            result = planner_success;
        }

#ifndef TEST_EXECUTOR
        // TODO Should be executed somewhere else
        executePlan(&plan, cncPosition);
#else
        Point lastPointForRender = convertPointFromStepsSize_line(lastPoint, stepSizes);
        executePlan_test(&plan, result, lastPointForRender, cncPosition, 0);

#endif
        if (result == planner_success) {
            break;
        }
    } while(1);

    // TODO Doing this we ignore to discrete movements error. Should be reported maybe? Should not accumulate for sure
    if (!isnan(x)) {
        cncPosition->x.pos = x;
    }
    if (!isnan(y)) {
        cncPosition->y.pos = y;
    }
    if (!isnan(z)) {
        cncPosition->z.pos = z;
    }
}

void executeLinearMovement(float x, float y, float z, CNCPosition *cncPosition) {
    do {
        Point from = {
                .x = cncPosition->x.pos,
                .y = cncPosition->y.pos
        };
        Point to = {
                .x = x,
                .y = y
        };
        Point stepSizes = {
                .x = cncPosition->x.stepSize,
                .y = cncPosition->y.stepSize,
        };

        float dx = fabs(to.x - from.x);
        float dy = fabs(to.y - from.y);
        float dzRaw = z - cncPosition->z.pos;
        float dz = fabs(z - cncPosition->z.pos);

        int plan_i = 0;
        Plan plan = {};
        Point lastPoint = to;
        enum PlannerResult result = planner_fail;

        if ((dx || dy) && !isnan(dx) && !isnan(dy)) {
            convert_coords_to_bresenham_line_2d(&from, &to, stepSizes);

            result = bresenham_line_2d(from, to, &plan, &lastPoint);
        } else{
            if (dz) {
//            float virtZ = cncPosition->z.pos;
                // TODO Simple movement suitable only for XY-plotter
//            while(virtZ < z) {
//                plan.items[plan_i++] = (PlanItem) {
//                    .type = z_move,
//                    .direction = dzRaw > 0 ? plan_item_dir_forward : plan_item_dir_backward
//                };
//                int modifier = dzRaw > 0 ? 1 : -1;
//                virtZ += modifier * cncPosition->z.stepSize;
//            }
            }

            result = planner_success;
        }

#ifndef TEST_EXECUTOR
        // TODO Should be executed somewhere else
        executePlan(&plan, cncPosition);
#else
        Point lastPointForRender = convertPointFromStepsSize_line(lastPoint, stepSizes);
        executePlan_test(&plan, result, lastPointForRender, cncPosition, 1);
#endif
        if (result == planner_success) {
            break;
        }
    } while(1);

    // TODO Doing this we ignore to discrete movements error. Should be reported maybe? Should not accumulate for sure
    if (!isnan(x)) {
        cncPosition->x.pos = x;
    }
    if (!isnan(y)) {
        cncPosition->y.pos = y;
    }
    if (!isnan(z)) {
        cncPosition->z.pos = z;
    }
}


void executePlan_test(Plan *plan, enum PlannerResult result, Point lastPoint, CNCPosition *cncPosition, char renderType) {
#ifdef TEST_EXECUTOR
    Point stepSizes = {
            .x = cncPosition->x.stepSize,
            .y = cncPosition->y.stepSize,
    };
    Point renderPos = {
            .x = cncPosition->x.pos,
            .y = cncPosition->y.pos,
    };

    if (!isnan(lastPoint.x)) {
        cncPosition->x.pos = lastPoint.x;
    }
    if (!isnan(lastPoint.y)) {
        cncPosition->y.pos = lastPoint.y;
    }

    if (renderType != 0) {
        addPlanToRender(plan, &renderPos, stepSizes.x, renderType);
    }
#endif
}

void executePlan(Plan *plan, CNCPosition *cncPosition) {
    for (int i = 0; i < PLAN_SIZE; ++i) {
        PlanItem item = plan->items[i];
        if (item.type == none) {
            break;
        }

        AxisState *axisState;
        if (item.type == x_move) {
            axisState = &cncPosition->x;
        } else if (item.type == y_move) {
            axisState = &cncPosition->y;
        } else {
            // TODO WTF
            continue;
        }

        setStepperDir(&axisState->stepState, item.direction == plan_item_dir_forward ? CW : CCW);
        makeStepperStep(&axisState->stepState);
        axisState->pos += item.direction == plan_item_dir_forward
            ? axisState->stepSize
            : -axisState->stepSize;
    }
}
