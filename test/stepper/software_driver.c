//
// Created by Michael Lipinski on 17/01/2018.
//

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <stepper/software_driver.h>
#include "./software_driver.h"

START_TEST (test_state_initialized)
{
    attachStepper(0, FULL_STEP_SINGLE_PHASE);

    StepState state = getStepperState(0);

    ck_assert_msg(state.attachMode == ATTACHED, "Should be attached");
}
END_TEST

START_TEST (test_state_not_initialized_if_stepper_id_too_big)
{
    attachStepper(123, FULL_STEP_SINGLE_PHASE);

    StepState state = getStepperState(123);
    ck_assert_msg(state.attachMode == DETACHED, "Should not be attached");
}
END_TEST

START_TEST (test_state_initialized_properly)
{
    attachStepper(1, FULL_STEP_SINGLE_PHASE);

    StepState state = getStepperState(1);

    ck_assert(state.direction == CW);
    ck_assert(state.phase == 0);
    ck_assert(state.phasesCount == 4);
    ck_assert(state.mode == FULL_STEP_SINGLE_PHASE);
    ck_assert(state.step == 0);
    ck_assert(state.attachMode == ATTACHED);

    attachStepper(2, HALF_STEP);

    StepState state2 = getStepperState(2);

    ck_assert(state2.direction == CW);
    ck_assert(state2.phase == 0);
    ck_assert(state2.phasesCount == 8);
    ck_assert(state2.mode == HALF_STEP);
    ck_assert(state2.step == 0);
    ck_assert(state2.attachMode == ATTACHED);
}
END_TEST

START_TEST (test_state_direction_changed_correctly)
{
    StepState state = getStepperState(1);

    ck_assert(state.direction == CW);

    setDirection(1, CCW);
    StepState state2 = getStepperState(1);

    ck_assert(state2.direction == CCW);
}
END_TEST

START_TEST (test_state_make_step_changes_state_correctly)
{
    unsigned char stepperId = 3;
    attachStepper(stepperId, FULL_STEP_SINGLE_PHASE);

    makeStep(stepperId);
    StepState state = getStepperState(stepperId);
    ck_assert(state.phase == 1);

    makeStep(stepperId);
    StepState state1 = getStepperState(stepperId);
    ck_assert(state1.phase == 2);

    makeStep(stepperId);
    StepState state2 = getStepperState(stepperId);
    ck_assert(state2.phase == 3);

    makeStep(stepperId);
    StepState state3 = getStepperState(stepperId);
    ck_assert(state3.phase == 0);

    makeStep(stepperId);
    StepState state4 = getStepperState(stepperId);
    ck_assert(state4.phase == 1);
}
END_TEST

START_TEST (test_state_make_step_changes_state_correctly_in_CCW)
{
    unsigned char stepperId = 4;
    attachStepper(stepperId, FULL_STEP_SINGLE_PHASE);
    setDirection(stepperId, CCW);

    makeStep(stepperId);
    StepState state = getStepperState(stepperId);
    ck_assert_uint_eq(state.phase, 3);

    makeStep(stepperId);
    StepState state1 = getStepperState(stepperId);
    ck_assert_uint_eq(state1.phase, 2);

    makeStep(stepperId);
    StepState state2 = getStepperState(stepperId);
    ck_assert_uint_eq(state2.phase, 1);

    makeStep(stepperId);
    StepState state3 = getStepperState(stepperId);
    ck_assert_uint_eq(state3.phase, 0);

    makeStep(stepperId);
    StepState state4 = getStepperState(stepperId);
    ck_assert_uint_eq(state4.phase, 3);
}
END_TEST

START_TEST (test_state_make_step_)
{
    unsigned char stepperId = 4;
    attachStepper(stepperId, FULL_STEP_SINGLE_PHASE);
    setDirection(stepperId, CCW);

    makeStep(stepperId);
    StepState state = getStepperState(stepperId);
    ck_assert_uint_eq(state.phase, 3);

    makeStep(stepperId);
    StepState state1 = getStepperState(stepperId);
    ck_assert_uint_eq(state1.phase, 2);

    makeStep(stepperId);
    StepState state2 = getStepperState(stepperId);
    ck_assert_uint_eq(state2.phase, 1);

    makeStep(stepperId);
    StepState state3 = getStepperState(stepperId);
    ck_assert_uint_eq(state3.phase, 0);

    makeStep(stepperId);
    StepState state4 = getStepperState(stepperId);
    ck_assert_uint_eq(state4.phase, 3);
}
END_TEST

START_TEST (test_state_get_pins_works)
{
    unsigned char stepperId = 5;
    unsigned char pins;

    attachStepper(stepperId, FULL_STEP_SINGLE_PHASE);

    pins = getPinsValues(stepperId);
    ck_assert_uint_eq(pins, 0b0110);

    makeStep(stepperId);
    pins = getPinsValues(stepperId);
    ck_assert_uint_eq(pins, 0b0101);

    makeStep(stepperId);
    pins = getPinsValues(stepperId);
    ck_assert_uint_eq(pins, 0b1001);

    makeStep(stepperId);
    pins = getPinsValues(stepperId);
    ck_assert_uint_eq(pins, 0b1010);

    makeStep(stepperId);
    pins = getPinsValues(stepperId);
    ck_assert_uint_eq(pins, 0b0110);

    makeStep(stepperId);
    pins = getPinsValues(stepperId);
    ck_assert_uint_eq(pins, 0b0101);

    setDirection(stepperId, CCW);
    makeStep(stepperId);
    pins = getPinsValues(stepperId);
    ck_assert_uint_eq(pins, 0b0110);
}
END_TEST

void fillSuite_stepper_software_driver(Suite* suite) {
    TCase *tcase = tcase_create("attachStepper'n'getStepperState");
    tcase_add_test(tcase, test_state_initialized);
    tcase_add_test(tcase, test_state_not_initialized_if_stepper_id_too_big);
    tcase_add_test(tcase, test_state_initialized_properly);
    suite_add_tcase(suite, tcase);

    TCase *tcase2 = tcase_create("setDirection");
    tcase_add_test(tcase2, test_state_direction_changed_correctly);
    suite_add_tcase(suite, tcase2);

    TCase *tcase3 = tcase_create("makeStep");
    tcase_add_test(tcase3, test_state_make_step_changes_state_correctly);
    tcase_add_test(tcase3, test_state_make_step_changes_state_correctly_in_CCW);
    suite_add_tcase(suite, tcase3);

    TCase *tcase4 = tcase_create("getPinsValues");
    tcase_add_test(tcase4, test_state_get_pins_works);
    suite_add_tcase(suite, tcase4);
}