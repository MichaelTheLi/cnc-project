//
// Created by Michael Lipinski on 13/02/2018.
//

#include <servo/driver.h>

#include "driver_test.h"

char initialized = 0;
float lastAngle = 0;

void initializeTestServo(ServoInnerState innerState) {
    initialized = 1;
}

void turnTestServo(ServoInnerState innerState) {
    lastAngle = innerState.currentAngle;
}

START_TEST(test_initialized_correctly)
    {
        ServoState state = composeServoState(&initializeTestServo, &turnTestServo);

        ck_assert_int_eq(initialized, 0);
        initializeServo(&state);

        ck_assert_int_eq(initialized, 1);
        ck_assert_float_eq(state.innerState.currentAngle, 0.0f);
    }
END_TEST

START_TEST(test_goToAngle_works_correctly)
    {
        ServoState state = composeServoState(&initializeTestServo, &turnTestServo);

        goToAngle(&state, 172.31);

        ck_assert_float_eq(lastAngle, 172.31);

        ck_assert_float_eq(state.innerState.currentAngle, 172.31);
    }
END_TEST

void fillSuite_servo_driver(Suite* suite) {
    TCase *tcase = tcase_create("Servo driver interface tests");

    tcase_add_test(tcase, test_initialized_correctly);
    tcase_add_test(tcase, test_goToAngle_works_correctly);
    tcase_set_timeout(tcase, 10);

    suite_add_tcase(suite, tcase);
}