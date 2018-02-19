//
// Created by Michael Lipinski on 19/02/2018.
//

#include "uart_test.h"

#include <serial/uart.h>

char testCommand[BUFFER_SIZE] = {};

void testCommandReady(char* command){
    strncpy((char *) testCommand, (const char *) command, BUFFER_SIZE);
}

bool testIsReady(){
    return true;
}

char testBuffer[256] = {};
int buffer_i = 0;
void testSendChar(char toSend){
    testBuffer[buffer_i++] = (char) (toSend + 1);
}

START_TEST(test_put_char_works)
    {
        serial_initialize(&testCommandReady, &testIsReady, &testSendChar);
        serial_put_char('Y');
        serial_put_char('G');
        serial_put_char(',');

        ck_assert_int_eq(testBuffer[0], 'Y' + 1);
        ck_assert_int_eq(testBuffer[1], 'G' + 1);
        ck_assert_int_eq(testBuffer[2], ',' + 1);
    }
END_TEST

START_TEST(test_put_string_works)
    {
        char *testString = "Hello world!";
        serial_initialize(&testCommandReady, &testIsReady, &testSendChar);
        serial_put_string(testString);

        ck_assert_int_eq(strlen(testBuffer), 12);
        for (int i = 0; i < strlen(testString); ++i) {
            ck_assert_int_eq(testBuffer[i], testString[i] + 1);
        }
    }
END_TEST

START_TEST(test_command_ready_callback_works)
    {
        char *testString = "Hello world command!\n";
        serial_initialize(&testCommandReady, &testIsReady, &testSendChar);

        for (int i = 0; i < strlen(testString); ++i) {
            ck_assert_int_eq(strlen(testCommand), 0); // Should be 0 until very last iteration with '\n'
            serial_onReceivedChar(testString[i]);
        }

        ck_assert_int_eq(strlen(testCommand), 21);
        for (int i = 0; i < strlen(testCommand); ++i) {
            ck_assert_int_eq(testCommand[i], testString[i]);
        }
    }
END_TEST

void fillSuite_uart_core(Suite* suite) {
    TCase *tcase = tcase_create("Serial lib suite");

    tcase_add_test(tcase, test_put_char_works);
    tcase_add_test(tcase, test_put_string_works);
    tcase_add_test(tcase, test_command_ready_callback_works);
    tcase_set_timeout(tcase, 10);

    suite_add_tcase(suite, tcase);
}