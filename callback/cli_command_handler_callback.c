/****************************************************************************************************
 * FILE:    cli_command_handler_callback.c
 * BRIEF:   Command Line Interface (CLI) Command Handler Callback Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "cli.h"
#include "cli_callback.h"
#include "cli_command_handler_callback.h"
#include "cs.h"
#include "system.h"
#include "status.h"
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "system.h"

/****************************************************************************************************
 * Constants and Variables
 ****************************************************************************************************/

static cli_record_t cliCommandHandlerCallback_systemCommandRecord;

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

static status_t cliCommandHandlerCallback_systemCommandHandlerCallback(uint8_t argc, char *argv[]);

/****************************************************************************************************
 * Function Definitions (Public)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   cliCommandHandlerCallback_init
 * BRIEF:   Initialize
 * RETURN:  Returns Nothing
 * ARG:     No Arguments
 ****************************************************************************************************/
void cliCommandHandlerCallback_init(void)
{
    /*** Register Commands ***/
    (void)cli_registerCommand(&cliCommandHandlerCallback_systemCommandRecord, CLI_COMMAND_HANDLER_CALLBACK_SYSTEM_COMMAND_NAME, cliCommandHandlerCallback_systemCommandHandlerCallback, NULL);
}

/****************************************************************************************************
 * Function Definitions (Private)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   cliCommandHandlerCallback_systemCommandHandlerCallback
 * BRIEF:   System Command Handler Callback
 * RETURN:  status_t: Success Or Failure Status
 * ARG:     argc: Number Of Arguments
 * ARG:     argv: Argument List
 ****************************************************************************************************/
static status_t cliCommandHandlerCallback_systemCommandHandlerCallback(uint8_t argc, char *argv[])
{
    bool clock, help, reset;
    uint32_t i;
    cli_optionArgumentPair_t optionArgumentPair;

    /*** Set Defaults ***/
    clock = false;
    help = false;
    reset = false;

    /*** Process Arguments ***/
    for(i = 0; i < argc; i++)
    {
        if(cli_getOptionArgumentPairFromInput(argv[i], &optionArgumentPair) == STATUS_SUCCESS)
        {
            if((strcmp(optionArgumentPair.option, "c") == 0) || (strcmp(optionArgumentPair.option, "clock") == 0))
            {
                /* Clock */
                if(optionArgumentPair.argument == NULL)
                    clock = true;
            }
            else if((strcmp(optionArgumentPair.option, "h") == 0) || (strcmp(optionArgumentPair.option, "help") == 0))
            {
                /* Help */
                if(optionArgumentPair.argument == NULL)
                    help = true;
            }
            else if((strcmp(optionArgumentPair.option, "r") == 0) || (strcmp(optionArgumentPair.option, "reset") == 0))
            {
                /* Reset */
                if(optionArgumentPair.argument == NULL)
                    reset = true;
            }
        }
    }

    /*** Handle Arguments ***/
    /* Clock */
    if(clock)
    {
        cliCallback_printfCallback(false, "Auxiliary Clock (ACLK): %lu Hz\n", CS_getACLK());
        cliCallback_printfCallback(false, "Master Clock (MCLK): %lu Hz\n", CS_getMCLK());
        cliCallback_printfCallback(true, "Sub-Main CLock (SMCLK): %lu Hz\n", CS_getSMCLK()); // Flush
    }

    /* Help */
    if(help)
    {
        cliCallback_printfCallback(false, "usage: %s [OPTION]\n", CLI_COMMAND_HANDLER_CALLBACK_SYSTEM_COMMAND_NAME);
        cliCallback_printfCallback(false, "  -c, --clock (Clock Information)\n");
        cliCallback_printfCallback(false, "  -h, --help (Help)\n");
        cliCallback_printfCallback(true, "  -r, --reset (Reset)\n"); // Flush
    }

    /* Reset (Must Come Last) */
    if(reset)
    {
        cliCallback_printfCallback(true, "Reset System\n"); // Flush
        cliCallback_printfCallback(true, ""); // Flush; Wait Until Message Above Printed Before Resetting
        system_reset();
    }

    return STATUS_SUCCESS;
}
