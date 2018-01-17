//
// Created by Michael Lipinski on 17/01/2018.
//

#include "parser.h"

START_TEST (test_parser)
{
    ck_assert_uint_eq(5, 2);
}
END_TEST

void fillSuite_gcode_parser(Suite* suite) {
    TCase *tcase = tcase_create("gcode parser");
    tcase_add_test(tcase, test_parser);
    suite_add_tcase(suite, tcase);
}