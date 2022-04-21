/****************************************************************************************************
 * FILE:    cli_callback_test.h
 * BRIEF:   Command Line Interface (CLI) Callback Test Header File
 ****************************************************************************************************/

#ifndef CLI_CALLBACK_TEST_H
#define CLI_CALLBACK_TEST_H

/****************************************************************************************************
 * Defines
 ****************************************************************************************************/

#define CLI_CALLBACK_TEST_PRINTF_OUTPUT_LENGTH (256)

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

extern void cliCallbackTest_copyPrintfOutput(const char * const Output);
extern void cliCallbackTest_getPrintfOutputCopy(char * const copy);
extern void cliCallbackTest_init(void);
extern void cliCallbackTest_sendCommand(const char * const Command);

#endif
