//
// Created by Michael Lipinski on 27/01/2018.
//

#include "hardware_driver_test.h"
#include <stepper/driver/hardware_driver.h>
#include <bits.h>

uint8_t testPort_hw = 0x00;
uint8_t *testPortPtr_hw = &testPort_hw;

START_TEST (test_create_works_correctly)
    {
        StepState_hw_popolu_A4988 state = createStepper_hw(
                FULL_STEP_DOUBLE_PHASE,
                testPortPtr_hw,
                0,
                1,
                2,
                3,
                4
        );

        ck_assert(state.mode == FULL_STEP_DOUBLE_PHASE);
        ck_assert_ptr_eq(state.port, testPortPtr_hw);

        ck_assert_int_eq(state.step_pin, 0);
        ck_assert_int_eq(state.dir_pin, 1);

        ck_assert_int_eq(state.ms1pin, 2);
        ck_assert_int_eq(state.ms2pin, 3);
        ck_assert_int_eq(state.ms3pin, 4);
    }
END_TEST

START_TEST (test__makeStepByPtr_hw__correctly_sets_microspetting_bits)
    {
        StepState_hw_popolu_A4988 state = createStepper_hw(
                FULL_STEP_DOUBLE_PHASE,
                testPortPtr_hw,
                5,
                6,
                1,
                2,
                3
        );

        makeStepByPtr_hw(&state);
        uint8_t expected = 0x00;
        bit_set(expected, BIT(6)); // DIR

        bit_clear(expected, BIT(1)); // ms1
        bit_clear(expected, BIT(2)); // ms2
        bit_clear(expected, BIT(3)); // ms3
        ck_assert_int_eq(*testPortPtr_hw, expected);

        setModeByPtr_hw(&state, QUARTER_STEP);
        makeStepByPtr_hw(&state);
        expected = 0x00;
        bit_set(expected, BIT(6)); // DIR
        bit_clear(expected, BIT(1)); // ms1
        bit_set(expected, BIT(2)); // ms2
        bit_clear(expected, BIT(3)); // ms3
        ck_assert_int_eq(*testPortPtr_hw, expected);

        setDirectionByPtr_hw(&state, CW);
        setModeByPtr_hw(&state, HALF_STEP);
        makeStepByPtr_hw(&state);
        expected = 0x00;
        bit_set(expected, BIT(6)); // DIR
        bit_set(expected, BIT(1)); // ms1
        bit_clear(expected, BIT(2)); // ms2
        bit_clear(expected, BIT(3)); // ms3
        ck_assert_int_eq(*testPortPtr_hw, expected);
    }
END_TEST

START_TEST (test__makeStepByPtr_hw__correctly_sets_direction_bits)
    {
        StepState_hw_popolu_A4988 state = createStepper_hw(
                FULL_STEP_DOUBLE_PHASE,
                testPortPtr_hw,
                5,
                6,
                1,
                2,
                3
        );

        makeStepByPtr_hw(&state);
        uint8_t expected = 0x00;
        bit_set(expected, BIT(6)); // DIR
        ck_assert_int_eq(*testPortPtr_hw, expected);

        setDirectionByPtr_hw(&state, CCW);
        makeStepByPtr_hw(&state);
        expected = 0x00;
        bit_clear(expected, BIT(6)); // DIR
        ck_assert_int_eq(*testPortPtr_hw, expected);

        setDirectionByPtr_hw(&state, CW);
        makeStepByPtr_hw(&state);
        expected = 0x00;
        bit_set(expected, BIT(6)); // DIR
        ck_assert_int_eq(*testPortPtr_hw, expected);
    }
END_TEST

START_TEST (test__makeStepByPtr_hw__correctly_sets_step_bits)
    {
        StepState_hw_popolu_A4988 state = createStepper_hw(
                FULL_STEP_DOUBLE_PHASE,
                testPortPtr_hw,
                5,
                6,
                1,
                2,
                3
        );

        bitLogClear();
        BitLogItem *log = bits_log;

        makeStepByPtr_hw(&state);
        // Check dir_pin call
        ck_assert_ptr_eq(log[3].addr, testPortPtr_hw);
        ck_assert_int_eq(log[3].bit, BIT(6));
        ck_assert_int_eq(log[3].type, bl_set);

        // Check step_pin call
        ck_assert_ptr_eq(log[4].addr, testPortPtr_hw);
        ck_assert_int_eq(log[4].bit, BIT(5));
        ck_assert_int_eq(log[4].type, bl_set);

        // Check step_pin call
        ck_assert_ptr_eq(log[5].addr, testPortPtr_hw);
        ck_assert_int_eq(log[5].bit, BIT(5));
        ck_assert_int_eq(log[5].type, bl_clear);
    }
END_TEST

START_TEST (test__setDirectionByPtr_hw__correctly_mutate_state)
    {
        StepState_hw_popolu_A4988 state = createStepper_hw(
                FULL_STEP_DOUBLE_PHASE,
                testPortPtr_hw,
                5,
                6,
                1,
                2,
                3
        );

        setDirectionByPtr_hw(&state, CCW);
        ck_assert_int_eq(state.dir, CCW);

        setDirectionByPtr_hw(&state, CW);
        ck_assert_int_eq(state.dir, CW);
    }
END_TEST

START_TEST (test__setModeByPtr_hw__correctly_mutate_state)
    {
        StepState_hw_popolu_A4988 state = createStepper_hw(
                FULL_STEP_DOUBLE_PHASE,
                testPortPtr_hw,
                5,
                6,
                1,
                2,
                3
        );

        setModeByPtr_hw(&state, HALF_STEP);
        ck_assert_int_eq(state.mode, HALF_STEP);

        setModeByPtr_hw(&state, EIGHTH_STEP);
        ck_assert_int_eq(state.mode, EIGHTH_STEP);
    }
END_TEST

void fillSuite_stepper_hardware_driver(Suite* suite) {
    TCase *tcase = tcase_create("createStepper_hw");
    tcase_add_test(tcase, test_create_works_correctly);
    suite_add_tcase(suite, tcase);

    TCase *tcase_make_step = tcase_create("makeStepByPtr_hw");
    tcase_add_test(tcase_make_step, test__makeStepByPtr_hw__correctly_sets_microspetting_bits);
    tcase_add_test(tcase_make_step, test__makeStepByPtr_hw__correctly_sets_direction_bits);
    tcase_add_test(tcase_make_step, test__makeStepByPtr_hw__correctly_sets_step_bits);
    suite_add_tcase(suite, tcase_make_step);

    TCase *tcase_state_manipulations = tcase_create("check state manipuations");
    tcase_add_test(tcase_state_manipulations, test__setDirectionByPtr_hw__correctly_mutate_state);
    tcase_add_test(tcase_state_manipulations, test__setModeByPtr_hw__correctly_mutate_state);
    suite_add_tcase(suite, tcase_state_manipulations);
}