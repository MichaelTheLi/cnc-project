//
// Created by Michael Lipinski on 22/01/2018.
//

#include "executor.h"
#include "math.h"
#include "../planner/linear.h"
#include "../../test/plannerVisualizer.h"

#ifdef TEST_EXECUTOR
    #include "../../test/plannerVisualizer.h"
#endif

void executeLinearMovement(float x, float y, float z, CNCPosition *cncPosition);
void executePlan(Plan *plan, CNCPosition *cncPosition);
void executePlan_test(Plan *plan, enum PlannerResult result, Point *lastPoint, CNCPosition *cncPosition);

GCodeExecuteResult executeCommand(GCodeCommand *gCodeCommand, CNCPosition *cncPosition) {
    GCodeCommand command = *gCodeCommand;

    float x = command[COMMAND_INDEX('X')];
    float y = command[COMMAND_INDEX('Y')];
    float z = command[COMMAND_INDEX('Z')];

    if (command[COMMAND_INDEX('G')] == 1) {
        if (isnan(x) && isnan(y) && isnan(z)) {
            return gcode_execute_common_error; // TODO Make error more specific
        }

        executeLinearMovement(x, y, z, cncPosition);
    }

    return gcode_execute_success;
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
        Plan plan = {};
        Point lastPoint = to;
        convert_coords_to_bresenham_line_2d(&from, &to, stepSizes);

        enum PlannerResult result = bresenham_line_2d(from, to, &plan, &lastPoint);

#ifndef TEST_EXECUTOR
        // TODO Should be executed somewhere else
        executePlan(&plan, cncPosition);
#else
        executePlan_test(&plan, result, &lastPoint, cncPosition);

#endif
        if (result == planner_success) {
            break;
        }
    } while(1);

    // TODO Doing this we ignore to discrete movements error. Should be reported maybe? Should not accumulate for sure
    cncPosition->x.pos = x;
    cncPosition->y.pos = y;
}

void executePlan_test(Plan *plan, enum PlannerResult result, Point *lastPoint, CNCPosition *cncPosition) {
    Point stepSizes = {
            .x = cncPosition->x.stepSize,
            .y = cncPosition->y.stepSize,
    };
    Point renderPos = {
            .x = cncPosition->x.pos,
            .y = cncPosition->y.pos,
    };
    *lastPoint = convertPointFromStepsSize_line(*lastPoint, stepSizes);
    cncPosition->x.pos = lastPoint->x;
    cncPosition->y.pos = lastPoint->y;

    addPlanToRender(plan, &renderPos, stepSizes.x);
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
