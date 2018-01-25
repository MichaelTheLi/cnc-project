//
// Created by Michael Lipinski on 17/01/2018.
//

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <stepper/driver/software_driver.h>
#include "./software_driver.h"

uint8_t testPort = 0x00;
uint8_t *testPortPtr = &testPort;
ConnectedPins testPins = {
    .A1_pin = 0,
    .A2_pin = 1,
    .B1_pin = 2,
    .B2_pin = 3,
};

START_TEST (test_state_attach_returns)
{
    StepState* statePtr = attachStepper(0, FULL_STEP_SINGLE_PHASE, testPortPtr, testPins);

    ck_assert_ptr_nonnull(statePtr);
}
END_TEST

START_TEST (test_state_attached_has_correct_state)
    {
        StepState state = *attachStepper(0, FULL_STEP_SINGLE_PHASE, testPortPtr, testPins);

        ck_assert_msg(state.attachMode == ATTACHED, "Should be attached");
    }
END_TEST

START_TEST (test_state_attachStepper_null_if_stepper_id_too_big)
{
    StepState* statePtr = attachStepper(123, FULL_STEP_SINGLE_PHASE, testPortPtr, testPins);

    ck_assert_ptr_null(statePtr);
}
END_TEST

START_TEST (test_state_getStepperState)
    {
        StepState* statePtr = getStepperState(0);

        ck_assert_ptr_nonnull(statePtr);

        StepState state = *statePtr;

        ck_assert_msg(state.attachMode == ATTACHED, "Should be attached");
    }
END_TEST

START_TEST (test_state_getStepperState_if_stepper_id_too_big)
    {
        StepState* statePtr = getStepperState(123);

        ck_assert_ptr_null(statePtr);
    }
END_TEST

START_TEST (test_state_initialized_properly)
{
    attachStepper(1, FULL_STEP_SINGLE_PHASE, testPortPtr, testPins);

    StepState state = *getStepperState(1);

    ck_assert(state.direction == CW);
    ck_assert(state.phase == 0);
    ck_assert(state.phasesCount == 4);
    ck_assert(state.mode == FULL_STEP_SINGLE_PHASE);
    ck_assert(state.step == 0);
    ck_assert(state.attachMode == ATTACHED);

    attachStepper(2, HALF_STEP, testPortPtr, testPins);

    StepState state2 = *getStepperState(2);

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
    StepState state = *getStepperState(1);

    ck_assert(state.direction == CW);

    setDirection(1, CCW);
    StepState state2 = *getStepperState(1);

    ck_assert(state2.direction == CCW);
}
END_TEST

START_TEST (test_state_direction_by_ptr_changed_correctly)
    {
        StepState *state = getStepperState(1);

        ck_assert(state->direction == CW);

        setDirectionByPtr(state, CCW);
        StepState state2 = *getStepperState(1);

        ck_assert(state->direction == CCW);
        ck_assert(state2.direction == CCW);
    }
END_TEST

START_TEST (test_state_make_step_changes_state_correctly)
{
    unsigned char stepperId = 3;
    attachStepper(stepperId, FULL_STEP_SINGLE_PHASE, testPortPtr, testPins);

    makeStep(stepperId);
    StepState state = *getStepperState(stepperId);
    ck_assert(state.phase == 1);

    makeStep(stepperId);
    StepState state1 = *getStepperState(stepperId);
    ck_assert(state1.phase == 2);

    makeStep(stepperId);
    StepState state2 = *getStepperState(stepperId);
    ck_assert(state2.phase == 3);

    makeStep(stepperId);
    StepState state3 = *getStepperState(stepperId);
    ck_assert(state3.phase == 0);

    makeStep(stepperId);
    StepState state4 = *getStepperState(stepperId);
    ck_assert(state4.phase == 1);
}
END_TEST

START_TEST (test_state_make_step_changes_state_correctly_in_CCW)
{
    unsigned char stepperId = 4;
    attachStepper(stepperId, FULL_STEP_SINGLE_PHASE, testPortPtr, testPins);
    setDirection(stepperId, CCW);

    makeStep(stepperId);
    StepState state = *getStepperState(stepperId);
    ck_assert_uint_eq(state.phase, 3);

    makeStep(stepperId);
    StepState state1 = *getStepperState(stepperId);
    ck_assert_uint_eq(state1.phase, 2);

    makeStep(stepperId);
    StepState state2 = *getStepperState(stepperId);
    ck_assert_uint_eq(state2.phase, 1);

    makeStep(stepperId);
    StepState state3 = *getStepperState(stepperId);
    ck_assert_uint_eq(state3.phase, 0);

    makeStep(stepperId);
    StepState state4 = *getStepperState(stepperId);
    ck_assert_uint_eq(state4.phase, 3);
}
END_TEST

START_TEST (test_state_make_step_by_ptr_changes_state_correctly)
    {
        unsigned char stepperId = 3;
        StepState *stepState = attachStepper(stepperId, FULL_STEP_SINGLE_PHASE, testPortPtr, testPins);

        makeStepByPtr(stepState);
        StepState state = *getStepperState(stepperId);
        ck_assert(state.phase == 1);

        makeStepByPtr(stepState);
        StepState state1 = *getStepperState(stepperId);
        ck_assert(state1.phase == 2);

        makeStepByPtr(stepState);
        StepState state2 = *getStepperState(stepperId);
        ck_assert(state2.phase == 3);

        makeStepByPtr(stepState);
        StepState state3 = *getStepperState(stepperId);
        ck_assert(state3.phase == 0);

        makeStepByPtr(stepState);
        StepState state4 = *getStepperState(stepperId);
        ck_assert(state4.phase == 1);
    }
END_TEST

START_TEST (test_state_make_step_by_ptr_changes_state_correctly_in_CCW)
    {
        unsigned char stepperId = 4;
        StepState *stepState = attachStepper(stepperId, FULL_STEP_SINGLE_PHASE, testPortPtr, testPins);

        setDirection(stepperId, CCW);

        makeStepByPtr(stepState);
        StepState state = *getStepperState(stepperId);
        ck_assert_uint_eq(state.phase, 3);

        makeStepByPtr(stepState);
        StepState state1 = *getStepperState(stepperId);
        ck_assert_uint_eq(state1.phase, 2);

        makeStepByPtr(stepState);
        StepState state2 = *getStepperState(stepperId);
        ck_assert_uint_eq(state2.phase, 1);

        makeStepByPtr(stepState);
        StepState state3 = *getStepperState(stepperId);
        ck_assert_uint_eq(state3.phase, 0);

        makeStepByPtr(stepState);
        StepState state4 = *getStepperState(stepperId);
        ck_assert_uint_eq(state4.phase, 3);
    }
END_TEST

START_TEST (test_port_output_correct)
{
    unsigned char stepperId = 5;

    attachStepper(stepperId, FULL_STEP_SINGLE_PHASE, testPortPtr, testPins);

    ck_assert_uint_eq(testPort, 0b0110);

    makeStep(stepperId);
    ck_assert_uint_eq(testPort, 0b0101);

    makeStep(stepperId);
    ck_assert_uint_eq(testPort, 0b1001);

    makeStep(stepperId);
    ck_assert_uint_eq(testPort, 0b1010);

    makeStep(stepperId);
    ck_assert_uint_eq(testPort, 0b0110);

    makeStep(stepperId);
    ck_assert_uint_eq(testPort, 0b0101);

    setDirection(stepperId, CCW);
    makeStep(stepperId);
    ck_assert_uint_eq(testPort, 0b0110);
}
END_TEST


void fillSuite_stepper_software_driver(Suite* suite) {
    TCase *tcase = tcase_create("attachStepper");
    tcase_add_test(tcase, test_state_attach_returns);
    tcase_add_test(tcase, test_state_attached_has_correct_state);
    tcase_add_test(tcase, test_state_attachStepper_null_if_stepper_id_too_big);
    tcase_add_test(tcase, test_state_initialized_properly);
    suite_add_tcase(suite, tcase);

    TCase *tcase_getStepperState = tcase_create("getStepperState");
    tcase_add_test(tcase_getStepperState, test_state_getStepperState_if_stepper_id_too_big);
    tcase_add_test(tcase_getStepperState, test_state_getStepperState);
    suite_add_tcase(suite, tcase_getStepperState);

    TCase *tcase2 = tcase_create("setDirection");
    tcase_add_test(tcase2, test_state_direction_changed_correctly);
    suite_add_tcase(suite, tcase2);

    TCase *tcase3 = tcase_create("makeStep");
    tcase_add_test(tcase3, test_state_make_step_changes_state_correctly);
    tcase_add_test(tcase3, test_state_make_step_changes_state_correctly_in_CCW);
    suite_add_tcase(suite, tcase3);

    TCase *tcase2ByPtr = tcase_create("setDirectionByPtr");
    tcase_add_test(tcase2ByPtr, test_state_direction_by_ptr_changed_correctly);
    suite_add_tcase(suite, tcase2ByPtr);

    TCase *tcaseMakeStepByPtr = tcase_create("makeStepByPtr");
    tcase_add_test(tcaseMakeStepByPtr, test_state_make_step_by_ptr_changes_state_correctly);
    tcase_add_test(tcaseMakeStepByPtr, test_state_make_step_by_ptr_changes_state_correctly_in_CCW);
    suite_add_tcase(suite, tcaseMakeStepByPtr);

    TCase *tcase4ByPtr = tcase_create("portOutput");
    tcase_add_test(tcase4ByPtr, test_port_output_correct);
    suite_add_tcase(suite, tcase4ByPtr);
}