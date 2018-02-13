//
// Created by Michael Lipinski on 15/01/2018.
//

#include <check.h>

#define TEST_EXECUTOR

#include "./stepper/software_driver.h"
#include "stepper/hardware_driver_test.h"
#include "servo/pwm_calculator_test.h"
#include "./gcode/parser.h"
#include "./gcode/executorTest.h"

#include "plannerVisualizer.h"
#include "plannerFunctionalTest.h"

Suite* str_suite (void) {
    Suite *suite = suite_create("Main suite");

    fillSuite_stepper_software_driver(suite);
    fillSuite_stepper_hardware_driver(suite);
    fillSuite_gcode_parser(suite);
    fillSuite_gcode_executor(suite);

    fillSuite_pwm_calculator(suite);

    return suite;
}

int main (int argc, char *argv[]) {
    int number_failed;

    Suite *suite = str_suite();
    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    // TODO Functional test should be in separate target
//    testLinearInterpolation();
//    testArcInterpolation();
//    testArcCCWInterpolation();

//    testRealCommand();

//    startPlannerVisualization();

    return number_failed;
}
