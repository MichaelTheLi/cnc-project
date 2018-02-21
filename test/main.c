//
// Created by Michael Lipinski on 15/01/2018.
//

#include <check.h>

#include "./stepper/software_driver.h"
#include "./stepper/hardware_driver_test.h"
#include "./servo/pwm_calculator_test.h"
#include "./servo/driver_test.h"
#include "./gcode/parser.h"
#include "./gcode/executorTest.h"
#include "./serial/uart_test.h"

Suite* str_suite (void) {
    Suite *suite = suite_create("Main suite");

    fillSuite_stepper_software_driver(suite);
    fillSuite_stepper_hardware_driver(suite);
    fillSuite_gcode_parser(suite);
    fillSuite_gcode_executor(suite);

    fillSuite_pwm_calculator(suite);
    fillSuite_servo_driver(suite);

    fillSuite_uart_core(suite);

    return suite;
}

int main (int argc, char *argv[]) {
    int number_failed;

    Suite *suite = str_suite();
    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return number_failed;
}
