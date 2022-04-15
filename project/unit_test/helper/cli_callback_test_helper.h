/****************************************************************************************************
 * FILE:    cli_callback_test_helper.h
 * BRIEF:   Command Line Interface (CLI) Callback Test Helper Header File
 ****************************************************************************************************/

#ifndef CLI_CALLBACK_TEST_HELPER_H
#define CLI_CALLBACK_TEST_HELPER_H

/****************************************************************************************************
 * Defines
 ****************************************************************************************************/

#define CLI_CALLBACK_TEST_HELPER_PRINTF_OUTPUT_LENGTH (256)

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

extern void cliCallbackTestHelper_copyPrintfOutput(const char * const Output);
extern void cliCallbackTestHelper_getPrintfOutputCopy(char * const copy);
extern void cliCallbackTestHelper_init(void);
extern void cliCallbackTestHelper_sendCommand(const char * const Command);

#endif
