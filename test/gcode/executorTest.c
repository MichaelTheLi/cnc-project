//
// Created by Michael Lipinski on 22/01/2018.
//

#include <gcode/executor.h>
#include "./executorTest.h"

START_TEST(test_gcode_executor_executes)
    {
        float rawCommand[26] = {};
        rawCommand[COMMAND_INDEX('G')] = 1;
        rawCommand[COMMAND_INDEX('X')] = 1.234f;
        rawCommand[COMMAND_INDEX('Y')] = -2.345f;
        rawCommand[COMMAND_INDEX('Z')] = 3.456f;

        GCodeCommand command = rawCommand;

        AxisState xAxis = {
                .pos = 5.0,
                .stepState = attachStepper(0, FULL_STEP_DOUBLE_PHASE),
                .stepSize = 0.0001,
        };
        AxisState yAxis = {
                .pos = 6.0,
                .stepState = attachStepper(1, FULL_STEP_DOUBLE_PHASE),
                .stepSize = 0.0001,
        };
        AxisState zAxis = {
                .pos = 7.0,
                .stepState = attachStepper(2, FULL_STEP_DOUBLE_PHASE),
                .stepSize = 0.0007,
        };

        CNCPosition cncPosition = {
            .x = xAxis,
            .y = yAxis,
            .z = zAxis,
            .feedRate = 10.1,
        };

        GCodeExecuteResult result = executeCommand(&command, &cncPosition);

        ck_assert_msg(result == gcode_execute_success, "Didn't executed successfully");

        // Tolerance is stepSize aka AxisState.perStep
        ck_assert_float_eq_tol(cncPosition.x.pos, 1.234, 0.0001);
        ck_assert_float_eq_tol(cncPosition.y.pos, -2.345, 0.0001);
        ck_assert_float_eq_tol(cncPosition.z.pos, 3.456, 0.0007);
    }
END_TEST

void fillSuite_gcode_executor(Suite* suite) {
    TCase *tcase = tcase_create("gcode executor");

    tcase_add_test(tcase, test_gcode_executor_executes);

    suite_add_tcase(suite, tcase);
}