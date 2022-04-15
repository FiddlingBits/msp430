/****************************************************************************************************
 * FILE:    cli_callback_test_helper.c
 * BRIEF:   Command Line Interface (CLI) Callback Test Helper Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "cli.h"
#include "cli_callback_test_helper.h"
#include "status.h"
#include <string.h>
#include "unity_fixture.h"

/****************************************************************************************************
 * Constants and Variables
 ****************************************************************************************************/

static char cliCallbackTestHelper_printfOutput[CLI_CALLBACK_TEST_HELPER_PRINTF_OUTPUT_LENGTH];

/****************************************************************************************************
 * Function Definitions (Public)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   cliCallbackTestHelper_copyPrintfOutput
 * BRIEF:   Copy Printf Output
 * RETURN:  Returns Nothing
 * ARG:     Output: Printf Output
 ****************************************************************************************************/
void cliCallbackTestHelper_copyPrintfOutput(const char * const Output)
{
    /*** Copy Printf Output ***/
    (void)strcat(cliCallbackTestHelper_printfOutput, Output);
}

/****************************************************************************************************
 * FUNCT:   cliCallbackTestHelper_getPrintfOutputCopy
 * BRIEF:   Get Printf Output Copy
 * RETURN:  Returns Nothing
 * ARG:     copy: Printf Output Copy
 ****************************************************************************************************/
void cliCallbackTestHelper_getPrintfOutputCopy(char * const copy)
{
    /*** Get Printf Output Copy ***/
    (void)strncpy(copy, cliCallbackTestHelper_printfOutput, CLI_CALLBACK_TEST_HELPER_PRINTF_OUTPUT_LENGTH);
    cliCallbackTestHelper_printfOutput[0] = '\0';
}

/****************************************************************************************************
 * FUNCT:   cliCallbackTestHelper_init
 * BRIEF:   Initialize
 * RETURN:  Returns Nothing
 * ARG:     No Arguments
 ****************************************************************************************************/
void cliCallbackTestHelper_init(void)
{
    /*** Initialize ***/
    cliCallbackTestHelper_printfOutput[0] = '\0';
}

/****************************************************************************************************
 * FUNCT:   cliCallbackTestHelper_sendCommand
 * BRIEF:   Send Command
 * RETURN:  Returns Nothing
 * ARG:     Command: Command
 ****************************************************************************************************/
void cliCallbackTestHelper_sendCommand(const char * const Command)
{
    size_t i;
    
    /*** Send Command ***/
    /* Receive Character */
    for(i = 0; i < strlen(Command); i++)
        TEST_ASSERT_EQUAL_INT(STATUS_SUCCESS, cli_addReceiveCharacter(Command[i]));
    
    /* Process Input */
    TEST_ASSERT_EQUAL_INT(STATUS_SUCCESS, cli_processInput());
}
