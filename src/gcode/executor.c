//
// Created by Michael Lipinski on 22/01/2018.
//

#include "executor.h"
#include "math.h"
#include "../planner/linear.h"

void executeLinearMovement(float x, float y, float z, CNCPosition *cncPosition);
void executePlan(Plan *plan, CNCPosition *cncPosition);

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
                .x = cncPosition->x.pos * (1/cncPosition->x.stepSize),
                .y = cncPosition->y.pos * (1/cncPosition->x.stepSize)
        };
        Point to = {
                .x = x * (1/cncPosition->x.stepSize),
                .y = y * (1/cncPosition->x.stepSize)
        };
        Plan plan = {};
        enum PlannerResult result = bresenham_line_2d(from, to, &plan);

        executePlan(&plan, cncPosition);

        if (result == planner_success) {
            break;
        }
    } while(1);

    // TODO Doing this we ignore to discrete movements error. Should be reported maybe? Should not accumulate for sure
    cncPosition->x.pos = x;
    cncPosition->y.pos = y;
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
