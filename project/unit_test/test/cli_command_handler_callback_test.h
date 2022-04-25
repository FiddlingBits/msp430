/****************************************************************************************************
 * FILE:    cli_command_handler_callback_test.h
 * BRIEF:   Command Line Interface (CLI) Command Handler Callback Test Header File
 ****************************************************************************************************/

#ifndef CLI_COMMAND_HANDLER_CALLBACK_TEST_H
#define CLI_COMMAND_HANDLER_CALLBACK_TEST_H

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "cli_callback_test.h"
#include "cli_config.h"
#include <stdint.h>

/****************************************************************************************************
 * Type Definitions
 ****************************************************************************************************/

typedef struct cliCommandHandlerCallbackTest_commandTestData_s
{
    char command[CLI_CONFIG_INPUT_MAXIMUM_LENGTH];
    char expectedOutput[CLI_CALLBACK_TEST_PRINTF_OUTPUT_LENGTH];
} cliCommandHandlerCallbackTest_commandTestData_t;

/****************************************************************************************************
 * Constants And Variables
 ****************************************************************************************************/

extern uint16_t randomCallback_seed;

#endif
