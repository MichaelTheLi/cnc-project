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
        addPlanToRender(&output, &renderPos, stepSize);
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
            0.15 // 0.5mm step size
    );
    AxisState yAxis = initializeAxis_functional(
            initiateStepper(
                    hardware_polulu,
                    createTestStepperState_functional(testPort2_functional_ptr)
            ),
            0.15 // 0.5mm step size
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
//
//    int size = 9;
//    char *commands[] = {
//        "N10 G0 X0 Y40",
//        "N20 G0 X10 Y40",
//        "N30 G2 X30 Y40 I10 J0",
//        "N40 G3 X50 Y40 I10 J0",
//        "N50 G1 X50 Y20",
//        "N60 G3 X30 Y20 I-10 J0",
//        "N70 G2 X10 Y20 I-10 J0",
//        "N80 G3 X10 Y30 I0 J5",
//        "N90 G3 X10 Y40 I0 J5",
//    };

    int size = 195;
    char *commands[] = {
            "(D)",
            "N10 G0 Z2",
            "N20 G0 X0.233 Y0.444",
            "N30 G1 Z-1",
            "N40 G1 X6.206 Y0.444",
            "N50 G1 X6.206 Y30.437",
            "N60 G1 X0.233 Y30.437",
            "N70 G1 X0.233 Y29.401",
            "N80 G1 X2.116 Y29.401",
            "N90 G1 X2.116 Y1.477",
            "N100 G1 X0.233 Y1.477",
            "N110 G1 X0.233 Y0.444",
            "N120 G0 Z2",
            "N130 G0 X8.088 Y0.444",
            "N140 G1 Z-1",
            "N150 G1 X9.341 Y0.68",
            "N160 G1 X10.598 Y1.212",
            "N170 G1 X11.528 Y1.979",
            "N180 G1 X12.483 Y3.015",
            "N190 G1 X13.732 Y5.084",
            "N200 G1 X14.205 Y6.119",
            "N210 G1 X14.989 Y8.187",
            "N220 G1 X15.454 Y9.755",
            "N230 G1 X15.781 Y11.292",
            "N240 G1 X16.082 Y14.16",
            "N250 G1 X16.082 Y16.73",
            "N260 G1 X15.781 Y19.596",
            "N270 G1 X15.454 Y21.129",
            "N280 G1 X14.989 Y22.704",
            "N290 G1 X14.205 Y24.761",
            "N300 G1 X13.732 Y25.797",
            "N310 G1 X12.483 Y27.869",
            "N320 G1 X11.528 Y28.905",
            "N330 G1 X10.598 Y29.671",
            "N340 G1 X9.341 Y30.21",
            "N350 G1 X8.088 Y30.437",
            "N360 G1 X8.088 Y29.401",
            "N370 G1 X9.014 Y29.174",
            "N380 G1 X9.806 Y28.635",
            "N390 G1 X10.271 Y28.11",
            "N400 G1 X10.899 Y27.074",
            "N410 G1 X11.381 Y25.542",
            "N420 G1 X11.528 Y24.236",
            "N430 G1 X11.691 Y22.164",
            "N440 G1 X11.691 Y8.72",
            "N450 G1 X11.528 Y6.651",
            "N460 G1 X11.381 Y5.35",
            "N470 G1 X10.899 Y3.783",
            "N480 G1 X10.271 Y2.749",
            "N490 G1 X9.806 Y2.246",
            "N500 G1 X9.014 Y1.714",
            "N510 G1 X8.088 Y1.477",
            "N520 G1 X8.088 Y0.444",
            "(i)",
            "N530 G0 Z2",
            "N540 G0 X22.472 Y0.267",
            "N550 G1 Z-1",
            "N560 G1 X28.373 Y0.267",
            "N570 G1 X28.373 Y1.034",
            "N580 G1 X26.956 Y1.034",
            "N590 G1 X26.956 Y21.994",
            "N600 G1 X28.373 Y21.994",
            "N610 G1 X28.373 Y22.76",
            "N620 G1 X22.472 Y22.76",
            "N630 G1 X22.472 Y21.994",
            "N640 G1 X23.888 Y21.994",
            "N650 G1 X23.888 Y1.034",
            "N660 G1 X22.472 Y1.034",
            "N670 G1 X22.472 Y0.267",
            "(a)",
            "N680 G0 Z2",
            "N690 G0 X34.619 Y0.267",
            "N700 G1 Z-1",
            "N710 G1 X34.619 Y1.034",
            "N720 G1 X35.086 Y1.034",
            "N730 G1 X35.552 Y1.242",
            "N740 G1 X36.018 Y1.625",
            "N750 G1 X36.377 Y2.009",
            "N760 G1 X36.843 Y3.163",
            "N770 G1 X39.552 Y13.45",
            "N780 G1 X39.911 Y12.297",
            "N790 G1 X37.328 Y2.778",
            "N800 G1 X37.328 Y2.394",
            "N810 G1 X37.561 Y1.625",
            "N820 G1 X37.669 Y1.419",
            "N830 G1 X37.901 Y1.242",
            "N840 G1 X38.386 Y1.034",
            "N850 G1 X38.853 Y1.034",
            "N860 G1 X38.853 Y0.267",
            "N870 G1 X34.619 Y0.267",
            "N880 G0 Z2",
            "N890 G0 X39.211 Y5.114",
            "N900 G1 Z-1",
            "N910 G1 X39.444 Y6.089",
            "N920 G1 X41.56 Y6.089",
            "N930 G1 X41.793 Y5.114",
            "N940 G1 X39.211 Y5.114",
            "N950 G0 Z2",
            "N960 G0 X42.977 Y1.034",
            "N970 G1 Z-1",
            "N980 G1 X44.608 Y1.034",
            "N990 G1 X40.61 Y16.162",
            "N1000 G1 X42.386 Y22.76",
            "N1010 G1 X48.034 Y1.034",
            "N1020 G1 X49.679 Y1.034",
            "N1030 G1 X49.679 Y0.267",
            "N1040 G1 X42.977 Y0.267",
            "N1050 G1 X42.977 Y1.034",
            "(n)",
            "N1060 G0 Z2",
            "N1070 G0 X56.338 Y0.118",
            "N1080 G1 Z-1",
            "N1090 G1 X59.745 Y0.118",
            "N1100 G1 X59.745 Y0.887",
            "N1110 G1 X59.046 Y1.094",
            "N1120 G1 X58.813 Y1.271",
            "N1130 G1 X58.579 Y1.655",
            "N1140 G1 X58.454 Y2.039",
            "N1150 G1 X58.329 Y2.631",
            "N1160 G1 X58.329 Y16.191",
            "N1170 G1 X57.755 Y17.17",
            "N1180 G1 X57.755 Y2.631",
            "N1190 G1 X57.629 Y2.039",
            "N1200 G1 X57.521 Y1.655",
            "N1210 G1 X57.27 Y1.271",
            "N1220 G1 X57.037 Y1.094",
            "N1230 G1 X56.338 Y0.887",
            "N1240 G1 X56.338 Y0.118",
            "N1250 G0 Z2",
            "N1260 G0 X67.274 Y0.118",
            "N1270 G1 Z-1",
            "N1280 G1 X67.274 Y8.632",
            "N1290 G1 X61.036 Y22.604",
            "N1300 G1 X56.105 Y22.604",
            "N1310 G1 X56.105 Y21.838",
            "N1320 G1 X57.629 Y21.838",
            "N1330 G1 X67.274 Y0.118",
            "N1340 G0 Z2",
            "N1350 G0 X67.274 Y13.509",
            "N1360 G1 Z-1",
            "N1370 G1 X67.274 Y20.093",
            "N1380 G1 X67.403 Y20.66",
            "N1390 G1 X67.506 Y21.043",
            "N1400 G1 X67.739 Y21.455",
            "N1410 G1 X67.988 Y21.639",
            "N1420 G1 X68.694 Y21.838",
            "N1430 G1 X68.694 Y22.604",
            "N1440 G1 X65.285 Y22.604",
            "N1450 G1 X65.285 Y21.838",
            "N1460 G1 X65.982 Y21.639",
            "N1470 G1 X66.215 Y21.455",
            "N1480 G1 X66.447 Y21.043",
            "N1490 G1 X66.576 Y20.66",
            "N1500 G1 X66.68 Y20.093",
            "N1510 G1 X66.68 Y14.071",
            "N1520 G1 X67.274 Y13.509",
            "(a)",
            "N1530 G0 Z2",
            "N1540 G0 X74.941 Y0.267",
            "N1550 G1 Z-1",
            "N1560 G1 X74.941 Y1.034",
            "N1570 G1 X75.407 Y1.034",
            "N1580 G1 X75.873 Y1.242",
            "N1590 G1 X76.34 Y1.625",
            "N1600 G1 X76.699 Y2.009",
            "N1610 G1 X77.164 Y3.163",
            "N1620 G1 X79.873 Y13.45",
            "N1630 G1 X80.232 Y12.297",
            "N1640 G1 X77.649 Y2.778",
            "N1650 G1 X77.649 Y2.394",
            "N1660 G1 X77.882 Y1.625",
            "N1670 G1 X77.99 Y1.419",
            "N1680 G1 X78.223 Y1.242",
            "N1690 G1 X78.707 Y1.034",
            "N1700 G1 X79.174 Y1.034",
            "N1710 G1 X79.174 Y0.267",
            "N1720 G1 X74.941 Y0.267",
            "N1730 G0 Z2",
            "N1740 G0 X79.532 Y5.114",
            "N1750 G1 Z-1",
            "N1760 G1 X79.765 Y6.089",
            "N1770 G1 X81.881 Y6.089",
            "N1780 G1 X82.115 Y5.114",
            "N1790 G1 X79.532 Y5.114",
            "N1800 G0 Z2",
            "N1810 G0 X83.299 Y1.034",
            "N1820 G1 Z-1",
            "N1830 G1 X84.929 Y1.034",
            "N1840 G1 X80.931 Y16.162",
            "N1850 G1 X82.707 Y22.76",
            "N1860 G1 X88.356 Y1.034",
            "N1870 G1 X90 Y1.034",
            "N1880 G1 X90 Y0.267",
            "N1890 G1 X83.299 Y0.267",
            "N1900 G1 X83.299 Y1.034",
    };

    for (int i = 0; i < size; ++i) {
        char *commandStr = commands[i];

        GCodeCommand *commandPtr = createCommand();

        GCodeParseResult parseResult = parseString(commandStr, commandPtr);

        printf("%i: %c\n", i, parseResult);
        if (parseResult == gcode_parse_error) {
            break;
        }

        GCodeExecuteResult executeResult = executeCommand(commandPtr, cncPositionPtr);
    }
}