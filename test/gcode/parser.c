//
// Created by Michael Lipinski on 17/01/2018.
//

#include "./parser.h"
#include "gcode/parser.h"
#include "stdio.h"

START_TEST (test_parser_reads_int_letter)
{
    char *inputString = "N1 G01 X0.1 Y0.2";
    GCodeCommand *commandPtr = createCommand();
    parseString(inputString, commandPtr);

    GCodeCommand command = *commandPtr;

    float value = command[COMMAND_INDEX('G')];
    ck_assert_float_eq(value, 1.0);
}
END_TEST

START_TEST (test_parser_reads_float_letter)
    {
        char *inputString = "N1 G01 X0.1234567 Y3.221";
        GCodeCommand *commandPtr = createCommand();
        parseString(inputString, commandPtr);

        GCodeCommand command = *commandPtr;

        ck_assert_float_eq_tol(
                command[COMMAND_INDEX('X')],
                0.1234567,
                0.0000001 // TODO Check tolerance
        );

        ck_assert_float_eq_tol(
                command[COMMAND_INDEX('Y')],
                3.221,
                0.0000001
        );
    }
END_TEST

START_TEST (test_parser_reads_float_letter_exceed_tolerance)
    {
        char *inputString = "N1 G01 X51.123456789123 Y0.2";
        GCodeCommand *commandPtr = createCommand();
        parseString(inputString, commandPtr);

        GCodeCommand command = *commandPtr;

        float value = command[COMMAND_INDEX('X')];
        ck_assert_float_eq_tol(value, 51.1234567891, 0.0000000001); // TODO Check tolerance
    }
END_TEST

START_TEST (test_parser_can_process_comments)
    {
        // Should not be processed. Also, should not break other letters
        char *inputString = "N1 G02 X2.1234 Y0.2 (comment here)";
        GCodeCommand *commandPtr = createCommand();
        parseString(inputString, commandPtr);

        GCodeCommand command = *commandPtr;

        ck_assert_float_eq(command[COMMAND_INDEX('N')], 1);
        ck_assert_float_eq(command[COMMAND_INDEX('G')], 2);
        ck_assert_float_eq(command[COMMAND_INDEX('X')], 2.1234);
        ck_assert_float_eq(command[COMMAND_INDEX('Y')], 0.2);
    }
END_TEST

void fillSuite_gcode_parser(Suite* suite) {
    TCase *tcase = tcase_create("gcode parser");

    tcase_add_test(tcase, test_parser_reads_int_letter);
    tcase_add_test(tcase, test_parser_reads_float_letter);
    tcase_add_test(tcase, test_parser_reads_float_letter_exceed_tolerance);
    tcase_add_test(tcase, test_parser_can_process_comments);
    suite_add_tcase(suite, tcase);
}