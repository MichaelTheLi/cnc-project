//
// Created by Michael Lipinski on 22/01/2018.
//

#include <stepper/stepper.h>
#include <gcode/executor.h>
#include <stdlib.h>
#include "./executorTest.h"

uint8_t testPort1 = 0x00;
uint8_t testPort2 = 0x00;
uint8_t testPort3 = 0x00;
uint8_t *testPort1_ptr = &testPort1;
uint8_t *testPort2_ptr = &testPort2;
uint8_t *testPort3_ptr = &testPort3;

AxisState initializeAxis(StepperState stepState, float stepSize) {
    return (AxisState){
            .pos = 0.0,
            .stepState = stepState,
            .stepSize = stepSize,
    };
}

InnerStepperState createTestStepperState(uint8_t *port) {
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

CNCPosition createTestCNCPosition() {
    AxisState xAxis = initializeAxis(
            initiateStepper(
                    hardware_polulu,
                    createTestStepperState(testPort1_ptr)
            ),
            0.5 // 0.5mm step size
    );
    AxisState yAxis = initializeAxis(
            initiateStepper(
                    hardware_polulu,
                    createTestStepperState(testPort2_ptr)
            ),
            0.5 // 0.5mm step size
    );
    AxisState zAxis = initializeAxis(
            // TODO somehow this should be servo instead of stepper
            initiateStepper(
                    hardware_polulu,
                    createTestStepperState(testPort3_ptr)
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

START_TEST(test_gcode_executor_executes)
    {
        float rawCommand[26] = {};
        rawCommand[COMMAND_INDEX('G')] = 1;
        rawCommand[COMMAND_INDEX('X')] = 34.5f;
        rawCommand[COMMAND_INDEX('Y')] = -25.1f;

        GCodeCommand command = rawCommand;

        CNCPosition cncPosition = createTestCNCPosition();
        CNCPosition *cncPositionPtr = &cncPosition;

        GCodeExecuteResult result = executeCommand(&command, cncPositionPtr);

        ck_assert_msg(result == gcode_execute_success, "Didn't executed successfully");

        // Tolerance is stepSize aka AxisState.perStep
        float x = cncPositionPtr->x.pos;
        float y = cncPositionPtr->y.pos;
        ck_assert_float_eq_tol(x, 34.5f, 0.6f);
        ck_assert_float_eq_tol(y, -25.1f, 0.6f);
    }
END_TEST

START_TEST(test_gcode_executor_tolerances_ok)
    {
        for (int i = 0; i < 10; ++i) {
            float rawCommand[26] = {};
            rawCommand[COMMAND_INDEX('G')] = 1;
            float x = (float)rand()/(float)(RAND_MAX/50) - 100;
            float y = (float)rand()/(float)(RAND_MAX/50) - 100;
            rawCommand[COMMAND_INDEX('X')] = x;
            rawCommand[COMMAND_INDEX('Y')] = y;

            GCodeCommand command = rawCommand;

            CNCPosition cncPosition = createTestCNCPosition();
            CNCPosition *cncPositionPtr = &cncPosition;

            GCodeExecuteResult result = executeCommand(&command, cncPositionPtr);

            ck_assert_msg(result == gcode_execute_success, "Didn't executed successfully");

            // Tolerance is stepSize aka AxisState.perStep
            float x_new = cncPositionPtr->x.pos;
            float y_new = cncPositionPtr->y.pos;
            ck_assert_float_eq_tol(x_new, x, 0.6f);
            ck_assert_float_eq_tol(y_new, y, 0.6f);
        }
    }
END_TEST

void fillSuite_gcode_executor(Suite* suite) {
    TCase *tcase = tcase_create("gcode executor");

    tcase_add_test(tcase, test_gcode_executor_executes);
    tcase_add_test(tcase, test_gcode_executor_tolerances_ok);
    tcase_set_timeout(tcase, 10);

    suite_add_tcase(suite, tcase);
}