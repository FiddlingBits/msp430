/****************************************************************************************************
 * FILE:    cli_command_handler_callback_test.c
 * BRIEF:   Command Line Interface (CLI) Command Handler Callback Test Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "cli_callback_test.h"
#include "cli_command_handler_callback.h"
#include "cli_command_handler_callback_test.h"
#include "hw_memmap.h"
#include <string.h>
#include "system.h"
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
    /* Simulation */
    hwMemmap_init();
    
    /* Application */
    system_init();
    
    /* Test */
    cliCallbackTest_init();
}

TEST_TEAR_DOWN(cli_command_handler_callback_test)
{
}

/****************************************************************************************************
 * Tests
 ****************************************************************************************************/

TEST(cli_command_handler_callback_test, ledCommandHandlerCallback)
{
    const char *command, *expectedOutput;
    char actualOutput[CLI_CALLBACK_TEST_PRINTF_OUTPUT_LENGTH];
    size_t i;
    
    /*** Test Data ***/
    const cliCommandHandlerCallbackTest_commandTestData_t CommandTestData[] =
    {
        /*** Enable Blink ***/
        /* Success */
        {"led -e -l0 -o500 -O250\n", "Enable Blink: SUCCESS\n[root/]$ "},
        {"led --enable --led=1 --on=1500 --off=1250\n", "Enable Blink: SUCCESS\n[root/]$ "},
        
        /* Failure */
        {"led -enow -l0 -o500 -O250\n", "[root/]$ "},
        {"led -e -l2 -o500 -O250\n", "Enable Blink: FAILURE\n[root/]$ "},
        {"led -e -l0 -o2001 -O250\n", "Enable Blink: FAILURE\n[root/]$ "},
        {"led -e -l0 -o500 -O2001\n", "Enable Blink: FAILURE\n[root/]$ "},
        {"led -e -l0 -o0 -O0\n", "Enable Blink: FAILURE\n[root/]$ "},
        {"led --enable=please --led=0 --on=500 --off=250\n", "[root/]$ "},
        {"led --enable --led=2 --on=500 --off=250\n", "Enable Blink: FAILURE\n[root/]$ "},
        {"led --enable --led=0 --on=2001 --off=250\n", "Enable Blink: FAILURE\n[root/]$ "},
        {"led --enable --led=0 --on=500 --off=2001\n", "Enable Blink: FAILURE\n[root/]$ "},
        {"led --enable --led=0 --on=0 --off=0\n", "Enable Blink: FAILURE\n[root/]$ "},
        
        /*** Help ***/
        /* Success */
        {"led -h\n", "usage: led [OPTION]\n  -e, --enable (Enable Blink)\n  -h, --help (Help)\n  -l, --led (LED)\n  -o, --on (On Milliseconds)\n  -O, --off (Off Milliseconds)\n[root/]$ "},
        {"led --help\n", "usage: led [OPTION]\n  -e, --enable (Enable Blink)\n  -h, --help (Help)\n  -l, --led (LED)\n  -o, --on (On Milliseconds)\n  -O, --off (Off Milliseconds)\n[root/]$ "},
        
        /* Failure */
        {"led -hme\n", "[root/]$ "},
        {"led --help=please\n", "[root/]$ "}
    };
    size_t CommandTestDataLength = sizeof(CommandTestData) / sizeof(CommandTestData[0]);
    
    /********************************************************************************
     * Test 1: LED Command Handler
     ********************************************************************************/
    
    /*** Subtest 1: Run Tests ***/
    for(i = 0; i < CommandTestDataLength; i++)
    {
        /* Set Up */
        command = CommandTestData[i].command;
        expectedOutput = CommandTestData[i].expectedOutput;
        
        /* Send Command */
        cliCallbackTest_sendCommand(command);
        
        /* Verify Output As Expected */
        cliCallbackTest_getPrintfOutputCopy(actualOutput);
        TEST_ASSERT_EQUAL_STRING(expectedOutput, actualOutput);
    }
}

TEST(cli_command_handler_callback_test, systemCommandHandlerCallback)
{
    const char *command, *expectedOutput;
    char actualOutput[CLI_CALLBACK_TEST_PRINTF_OUTPUT_LENGTH];
    size_t i;
    
    /*** Test Data ***/
    const cliCommandHandlerCallbackTest_commandTestData_t CommandTestData[] =
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
        
        /*** Reset ***/
        /* Success */
        {"system -r\n", "Reset System\n[root/]$ "},
        {"system --reset\n", "Reset System\n[root/]$ "},
        
        /* Failure */
        {"system -rnow\n", "[root/]$ "},
        {"system --reset=10\n", "[root/]$ "}
    };
    size_t CommandTestDataLength = sizeof(CommandTestData) / sizeof(CommandTestData[0]);
    
    /********************************************************************************
     * Test 1: System Command Handler
     ********************************************************************************/
    
    /*** Subtest 1: Run Tests ***/
    for(i = 0; i < CommandTestDataLength; i++)
    {
        /* Set Up */
        command = CommandTestData[i].command;
        expectedOutput = CommandTestData[i].expectedOutput;
        
        /* Send Command */
        cliCallbackTest_sendCommand(command);
        
        /* Verify Output As Expected */
        cliCallbackTest_getPrintfOutputCopy(actualOutput);
        TEST_ASSERT_EQUAL_STRING(expectedOutput, actualOutput);
    }
}

/****************************************************************************************************
 * Test Group Runner
 ****************************************************************************************************/

TEST_GROUP_RUNNER(cli_command_handler_callback_test)
{
    RUN_TEST_CASE(cli_command_handler_callback_test, ledCommandHandlerCallback)
    RUN_TEST_CASE(cli_command_handler_callback_test, systemCommandHandlerCallback)
}
