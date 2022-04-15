/****************************************************************************************************
 * FILE:    cli_command_handler_callback_test.c
 * BRIEF:   Command Line Interface (CLI) Command Handler Callback Test Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "cli_callback_test_helper.h"
#include "cli_command_handler_callback.h"
#include "cli_command_handler_callback_test.h"
#include "hw_memmap.h"
#include <string.h>
#include "system.h"
#include "test_helper.h"
#include "unity_fixture.h"

/****************************************************************************************************
 * Test Group
 ****************************************************************************************************/

TEST_GROUP(cli_command_handler_callback_test);

/****************************************************************************************************
 * Test Setup And Teardown
 ****************************************************************************************************/

TEST_SETUP(cli_command_handler_callback_test)
{
    /*** Set Up ***/
    hwMemmap_init();
    system_init();
    testHelper_init();
}

TEST_TEAR_DOWN(cli_command_handler_callback_test)
{
}

/****************************************************************************************************
 * Tests
 ****************************************************************************************************/

TEST(cli_command_handler_callback_test, systemCommandHandlerCallback)
{
    const char *command, *expectedOutput;
    char actualOutput[CLI_CALLBACK_TEST_HELPER_PRINTF_OUTPUT_LENGTH];
    size_t i;
    
    /*** Test Data ***/
    const cliCommandHandlerCallbackTest_testData_t TestData[] =
    {
        /*** Clock ***/
        /* Success */
        {"system -c\n", "Auxiliary Clock (ACLK): 32768 Hz\nMaster Clock (MCLK): 8000000 Hz\nSub-Main CLock (SMCLK): 8000000 Hz\n[root/]$ "},
        {"system --clock\n", "Auxiliary Clock (ACLK): 32768 Hz\nMaster Clock (MCLK): 8000000 Hz\nSub-Main CLock (SMCLK): 8000000 Hz\n[root/]$ "},
        
        /* Failure */
        {"system -c5\n", "[root/]$ "},
        {"system --clock=info\n", "[root/]$ "},
        
        /*** Help ***/
        /* Success */
        {"system -h\n", "usage: system [OPTION]\n  -c, --clock (Clock Information)\n  -h, --help (Help)\n  -r, --reset (Reset)\n[root/]$ "},
        {"system --help\n", "usage: system [OPTION]\n  -c, --clock (Clock Information)\n  -h, --help (Help)\n  -r, --reset (Reset)\n[root/]$ "},
        
        /* Failure */
        {"system -hme\n", "[root/]$ "},
        {"system --help=please\n", "[root/]$ "},
        
        /*** Miscellaneous ***/
        {"system -x\n", "[root/]$ "},
        {"system --zebra\n", "[root/]$ "},
        {"system -yum\n", "[root/]$ "},
        {"system --zoo=zebra\n", "[root/]$ "},
        
        /*** Reset ***/
        /* Success */
        {"system -r\n", "Reset System\n[root/]$ "},
        {"system --reset\n", "Reset System\n[root/]$ "},
        
        /* Failure */
        {"system -rnow\n", "[root/]$ "},
        {"system --reset=10\n", "[root/]$ "}
    };
    size_t TestDataLength = sizeof(TestData) / sizeof(TestData[0]);
    
    /********************************************************************************
     * Test 1:  System Command Handler
     ********************************************************************************/
    
    /*** Subtest 1: Run Tests ***/
    for(i = 0; i < TestDataLength; i++)
    {
        /* Set Up */
        command = TestData[i].command;
        expectedOutput = TestData[i].expectedOutput;
        
        /* Send Command */
        cliCallbackTestHelper_sendCommand(command);
        
        /* Verify Output As Expected */
        cliCallbackTestHelper_getPrintfOutputCopy(actualOutput);
        TEST_ASSERT_EQUAL_STRING(expectedOutput, actualOutput);
    }
}

/****************************************************************************************************
 * Test Group Runner
 ****************************************************************************************************/

TEST_GROUP_RUNNER(cli_command_handler_callback_test)
{
    RUN_TEST_CASE(cli_command_handler_callback_test, systemCommandHandlerCallback)
}
