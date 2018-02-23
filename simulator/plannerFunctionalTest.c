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
#include <printf.h>
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
        result = bresenham_arc_2d(from, to, radius, &output, &lastPoint, ARC_CCW);

        Point renderPos = convertPointFromStepsSize(from, center, stepSizes);
        addPlanToRender(&output, &renderPos, stepSize, 1);
        if (result == planner_full) {
            from = lastPoint;
        }
    } while(result != planner_success && result != planner_fail);
}
void testArcCWInterpolation() {
    enum PlannerResult result;
    Point from = {
            .x = 0,
            .y = 0
    };
    Point to = {
            .x = -40,
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
        result = bresenham_arc_2d(from, to, radius, &output, &lastPoint, ARC_CW);

        Point renderPos = convertPointFromStepsSize(from, center, stepSizes);
        addPlanToRender(&output, &renderPos, stepSize, 1);
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
        addPlanToRender(&output, &renderPos, stepSize, 1);
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

float microstepLevel = 16.0f;

CNCPosition createTestCNCPosition_functional() {
    AxisState xAxis = initializeAxis_functional(
            initiateStepper(
                    hardware_polulu,
                    createTestStepperState_functional(testPort1_functional_ptr)
            ),
            0.15f / microstepLevel // 0.5mm step size
    );
    AxisState yAxis = initializeAxis_functional(
            initiateStepper(
                    hardware_polulu,
                    createTestStepperState_functional(testPort2_functional_ptr)
            ),
            0.15f / microstepLevel // 0.5mm step size
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
void testRealCommand(int size, char **commands) {
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

    int start = 0;
    int end = size;

//    int start = 0;
//    int end = size;
    if (start != 0) {
        char *commandStrInit = commands[start - 1];

        GCodeCommand *commandPtrInit = createCommand();

        parseString(commandStrInit, commandPtrInit);
        GCodeCommand parsed = *commandPtrInit;
        parsed[COMMAND_INDEX('G')] = 0;
        executeCommand(commandPtrInit, cncPositionPtr);
    }

    char* resultNames[] = {
            "gcode_execute_success",
            "gcode_execute_common_error",
            "gcode_g0_not_enough_params",
            "gcode_g1_not_enough_params",
            "gcode_g2_g3_no_radius",
    };

    for (int i = start; i < end; ++i) {
        char *commandStr = commands[i];

        GCodeCommand *commandPtr = createCommand();

        GCodeParseResult parseResult = parseString(commandStr, commandPtr);

        printf("%s | %i: %c\n", commandStr, i, parseResult);
        if (parseResult == gcode_parse_error) {
            printf("Parse error occurred");
            break;
        }

        GCodeExecuteResult executeResult = executeCommand(commandPtr, cncPositionPtr);

        if (executeResult != gcode_execute_success) {
            printf("Execute error occurred: %s\n", resultNames[executeResult]);
            break;
        }
    }
}