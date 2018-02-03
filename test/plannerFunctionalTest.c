//
// Created by Michael Lipinski on 03/02/2018.
//

#include <planner/planner.h>
#include <gcode/parser.h>
#include <core/core.h>
#include <gcode/executor.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "plannerFunctionalTest.h"

void testArcInterpolation() {
    enum PlannerResult result;
    Point from = {
            .x = 40,
            .y = 0
    };
    Point to = {
            .x = 0,
            .y = 0
    };

    float radius = 20;

    float stepSize = 0.01;
    Point stepSizes = {
            .x = stepSize,
            .y = stepSize,
    };

    Point origFrom = from;
    Point lastPoint = from;
    Point center = findCenter(origFrom, to, 20);

    convert_coords_to_bresenham_arc_2d(&from, &to, &radius, center, stepSizes);
    do {
        Plan output = {};
        result = bresenham_arc_2d(from, to, radius, &output, &lastPoint);

        Point renderPos = convertPointFromStepsSize(from, center, stepSizes);
        addPlanToRender(&output, &renderPos, stepSize);
        if (result == planner_full) {
            from = lastPoint;
        }
    } while(result != planner_success && result != planner_fail);
}

void testLinearInterpolation() {
    enum PlannerResult result;

    Point from = {
            .x = -50.2f,
            .y = 5.3f
    };
    Point to = {
            .x = 10.2f,
            .y = 2.3f
    };

    float stepSize = 0.01;
    Point stepSizes = {
            .x = stepSize,
            .y = stepSize,
    };

    addRealLine(&from, &to);
    Point lastPoint = to;

    convert_coords_to_bresenham_line_2d(&from, &to, stepSizes);
    do {
        Plan output = {};
        result = bresenham_line_2d(from, to, &output, &lastPoint);
        Point renderPos = convertPointFromStepsSize_line(from, stepSizes);
        addPlanToRender(&output, &renderPos, stepSize);
        if (result == planner_full) {
            from = lastPoint;
        }
    } while(result != planner_success && result != planner_fail);
}

uint8_t testPort1_functional = 0x00;
uint8_t testPort2_functional = 0x00;
uint8_t testPort3_functional = 0x00;
uint8_t *testPort1_functional_ptr = &testPort1_functional;
uint8_t *testPort2_functional_ptr = &testPort2_functional;
uint8_t *testPort3_functional_ptr = &testPort3_functional;

AxisState initializeAxis_functional(StepperState stepState, float stepSize) {
    return (AxisState){
            .pos = 0.0,
            .stepState = stepState,
            .stepSize = stepSize,
    };
}

InnerStepperState createTestStepperState_functional(uint8_t *port) {
    InnerStepperState state = {};
    state.h_state = createStepper_hw(
            FULL_STEP_DOUBLE_PHASE,
            port,
            0, // STEP pin
            1, // DIR pin
            2, // ms1 pin
            3, // ms2 pin
            4  // ms3 pin
    );

    return state;
}

CNCPosition createTestCNCPosition_functional() {
    AxisState xAxis = initializeAxis_functional(
            initiateStepper(
                    hardware_polulu,
                    createTestStepperState_functional(testPort1_functional_ptr)
            ),
            0.5 // 0.5mm step size
    );
    AxisState yAxis = initializeAxis_functional(
            initiateStepper(
                    hardware_polulu,
                    createTestStepperState_functional(testPort2_functional_ptr)
            ),
            0.5 // 0.5mm step size
    );
    AxisState zAxis = initializeAxis_functional(
            // TODO somehow this should be servo instead of stepper
            initiateStepper(
                    hardware_polulu,
                    createTestStepperState_functional(testPort3_functional_ptr)
            ),
            0.5 // 0.5mm step size
    );

    CNCPosition cncPosition = {
            .x = xAxis,
            .y = yAxis,
            .z = zAxis,
            .feedRate = 10.1,
    };

    return cncPosition;
}

CNCPosition *cncPositionPtr;

// TODO This should execute some complete g-code program
void testRealCommand() {
    if (cncPositionPtr == NULL) {
        CNCPosition cncPosition = createTestCNCPosition_functional();

        cncPositionPtr = malloc(sizeof *cncPositionPtr);
        if (cncPositionPtr == NULL) {
            /* failed to allocate, handle error here */
        } else {
            /* OK to copy.  Make sure you initialize 'hold' to something valid */
            memcpy(cncPositionPtr, &cncPosition, sizeof *cncPositionPtr);
        }

        cncPositionPtr = &cncPosition;
    }

    float rawCommand[26] = {};
    rawCommand[COMMAND_INDEX('G')] = 1;
    rawCommand[COMMAND_INDEX('X')] = -50;
    rawCommand[COMMAND_INDEX('Y')] = -25;

    GCodeCommand command = rawCommand;

    GCodeExecuteResult result = executeCommand(&command, cncPositionPtr);
}