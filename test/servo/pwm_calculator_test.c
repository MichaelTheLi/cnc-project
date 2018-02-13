//
// Created by Michael Lipinski on 13/02/2018.
//

#include <servo/pwm_calculator.h>
#include "pwm_calculator_test.h"

#define TEST_PRESCALER 8

START_TEST(test_pwm_calculator_calculates_zero_value_correctly)
    {
        int valueForZero = (int) calculateValueForZero(1500, TEST_PRESCALER);

        ck_assert_int_eq(valueForZero, 187);
    }
END_TEST

START_TEST(test_pwm_calculator_calculates_increment_per_degree_correctly)
    {
        float inc = calculateIncrementPerDegree(1500, 2000, TEST_PRESCALER);

        ck_assert_float_eq_tol(inc, 0.6944, 0.01 / 90);
    }
END_TEST

int calculateTestValueForAngle(int angle) {
    return (int) roundf(calculateValueForAngle(angle, 1500, 2000, TEST_PRESCALER));
}
START_TEST(test_pwm_calculator_calculates_value_for_angle_correctly)
    {
        int valueForAngle;

        valueForAngle = calculateTestValueForAngle(0);
        ck_assert_int_eq(valueForAngle, 187);

        valueForAngle = calculateTestValueForAngle(90);
        ck_assert_int_eq(valueForAngle, 250);

        valueForAngle = calculateTestValueForAngle(-90);
        ck_assert_int_eq(valueForAngle, 125);

        valueForAngle = calculateTestValueForAngle(350);
        ck_assert_int_eq(valueForAngle, 430);
    }
END_TEST

void fillSuite_pwm_calculator(Suite* suite) {
    TCase *tcase = tcase_create("PWM calculator for servo");

    tcase_add_test(tcase, test_pwm_calculator_calculates_zero_value_correctly);
    tcase_add_test(tcase, test_pwm_calculator_calculates_increment_per_degree_correctly);
    tcase_add_test(tcase, test_pwm_calculator_calculates_value_for_angle_correctly);
    tcase_set_timeout(tcase, 10);

    suite_add_tcase(suite, tcase);
}