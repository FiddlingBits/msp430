/****************************************************************************************************
 * FILE:    cli_command_handler_callback.c
 * BRIEF:   Command Line Interface (CLI) Command Handler Callback Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Defines
 ****************************************************************************************************/

#define CLI_COMMAND_HANDLER_CALLBACK_LED_COMMAND_NAME    ("led")
#define CLI_COMMAND_HANDLER_CALLBACK_RANDOM_COMMAND_NAME ("random")
#define CLI_COMMAND_HANDLER_CALLBACK_SYSTEM_COMMAND_NAME ("system")

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "cli.h"
#include "cli_callback.h"
#include "cli_command_handler_callback.h"
#include "cs.h"
#include "led_driver.h"
#include "random.h"
#include "random_callback.h"
#include "status.h"
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "system.h"

/****************************************************************************************************
 * Constants and Variables
 ****************************************************************************************************/

static cli_record_t cliCommandHandlerCallback_ledCommandRecord;
static cli_record_t cliCommandHandlerCallback_randomCommandRecord;
static cli_record_t cliCommandHandlerCallback_systemCommandRecord;

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

static status_t cliCommandHandlerCallback_ledCommandHandlerCallback(uint8_t argc, char *argv[]);
static status_t cliCommandHandlerCallback_randomCommandHandlerCallback(uint8_t argc, char *argv[]);
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
    (void)cli_registerCommand(&cliCommandHandlerCallback_ledCommandRecord, CLI_COMMAND_HANDLER_CALLBACK_LED_COMMAND_NAME, cliCommandHandlerCallback_ledCommandHandlerCallback, NULL);
    (void)cli_registerCommand(&cliCommandHandlerCallback_randomCommandRecord, CLI_COMMAND_HANDLER_CALLBACK_RANDOM_COMMAND_NAME, cliCommandHandlerCallback_randomCommandHandlerCallback, NULL);
    (void)cli_registerCommand(&cliCommandHandlerCallback_systemCommandRecord, CLI_COMMAND_HANDLER_CALLBACK_SYSTEM_COMMAND_NAME, cliCommandHandlerCallback_systemCommandHandlerCallback, NULL);
}

/****************************************************************************************************
 * Function Definitions (Private)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   cliCommandHandlerCallback_ledCommandHandlerCallback
 * BRIEF:   LED Command Handler Callback
 * RETURN:  status_t: Success Or Failure Status
 * ARG:     argc: Number Of Arguments
 * ARG:     argv: Argument List
 ****************************************************************************************************/
static status_t cliCommandHandlerCallback_ledCommandHandlerCallback(uint8_t argc, char *argv[])
{
    bool help, enableBlink;
    uint8_t i;
    uint16_t onMilliseconds, offMilliseconds;
    uint32_t u32;
    cli_optionArgumentPair_t optionArgumentPair;
    ledDriver_led_t led;

    /*** Set Defaults ***/
    help = false;
    enableBlink = false;
    led = LED_DRIVER_LED_COUNT; // Invalid
    onMilliseconds = (uint16_t)~0; // Invalid
    offMilliseconds = (uint16_t)~0; // Invalid

    /*** Process Arguments ***/
    for(i = 0; i < argc; i++)
    {
        if(cli_getOptionArgumentPairFromInput(argv[i], &optionArgumentPair) == STATUS_SUCCESS)
        {
            if((strcmp(optionArgumentPair.option, "e") == 0) || (strcmp(optionArgumentPair.option, "enable") == 0))
            {
                /* Enable Blink */
                if(optionArgumentPair.argument == NULL)
                    enableBlink = true;
            }
            else if((strcmp(optionArgumentPair.option, "h") == 0) || (strcmp(optionArgumentPair.option, "help") == 0))
            {
                /* Help */
                if(optionArgumentPair.argument == NULL)
                    help = true;
            }
            else if((strcmp(optionArgumentPair.option, "l") == 0) || (strcmp(optionArgumentPair.option, "led") == 0))
            {
                /* LED */
                if(optionArgumentPair.argument != NULL)
                {
                    (void)cli_getUnsigned32BitIntegerFromInput(optionArgumentPair.argument, &u32);
                    led = (ledDriver_led_t)u32;
                }
            }
            else if((strcmp(optionArgumentPair.option, "o") == 0) || (strcmp(optionArgumentPair.option, "on") == 0))
            {
                /* On Milliseconds */
                if(optionArgumentPair.argument != NULL)
                {
                    (void)cli_getUnsigned32BitIntegerFromInput(optionArgumentPair.argument, &u32);
                    onMilliseconds = (uint16_t)u32;
                }
            }
            else if((strcmp(optionArgumentPair.option, "O") == 0) || (strcmp(optionArgumentPair.option, "off") == 0))
            {
                /* Off Milliseconds */
                if(optionArgumentPair.argument != NULL)
                {
                    (void)cli_getUnsigned32BitIntegerFromInput(optionArgumentPair.argument, &u32);
                    offMilliseconds = (uint16_t)u32;
                }
            }
        }
    }

    /*** Handle Arguments ***/
    /* Help */
    if(help)
    {
        cliCallback_printfCallback(false, "usage: %s [OPTION]\n", CLI_COMMAND_HANDLER_CALLBACK_LED_COMMAND_NAME);
        cliCallback_printfCallback(false, "  -e, --enable (Enable Blink)\n");
        cliCallback_printfCallback(false, "  -h, --help (Help)\n");
        cliCallback_printfCallback(false, "  -l, --led (LED)\n");
        cliCallback_printfCallback(false, "  -o, --on (On Milliseconds)\n");
        cliCallback_printfCallback(true, "  -O, --off (Off Milliseconds)\n");
    }

    /* Enable Blink */
    if(enableBlink)
    {
    	cliCallback_printfCallback(false, "Enable Blink: ");
        if(ledDriver_enableBlink(led, onMilliseconds, offMilliseconds))
        	cliCallback_printfCallback(true, "SUCCESS\n"); // Flush
        else
        	cliCallback_printfCallback(true, "FAILURE\n"); // Flush
    }

    return STATUS_SUCCESS;
}

/****************************************************************************************************
 * FUNCT:   cliCommandHandlerCallback_randomCommandHandlerCallback
 * BRIEF:   Random Command Handler Callback
 * RETURN:  status_t: Success Or Failure Status
 * ARG:     argc: Number Of Arguments
 * ARG:     argv: Argument List
 ****************************************************************************************************/
static status_t cliCommandHandlerCallback_randomCommandHandlerCallback(uint8_t argc, char *argv[])
{
    bool help, seed, signedInteger, unsignedInteger;
    uint32_t count, i;
    cli_optionArgumentPair_t optionArgumentPair;

    /*** Set Defaults ***/
    count = 1;
    help = false;
    seed = false;
    signedInteger = false;
    unsignedInteger = false;

    /*** Process Arguments ***/
    for(i = 0; i < argc; i++)
    {
        if(cli_getOptionArgumentPairFromInput(argv[i], &optionArgumentPair) == STATUS_SUCCESS)
        {
            if((strcmp(optionArgumentPair.option, "c") == 0) || (strcmp(optionArgumentPair.option, "count") == 0))
            {
                /* Set Random 32-Bit Integer Count */
                if(optionArgumentPair.argument != NULL)
                    (void)cli_getUnsigned32BitIntegerFromInput(optionArgumentPair.argument, &count);
            }
            else if((strcmp(optionArgumentPair.option, "h") == 0) || (strcmp(optionArgumentPair.option, "help") == 0))
            {
                /* Help */
                if(optionArgumentPair.argument == NULL)
                    help = true;
            }
            else if((strcmp(optionArgumentPair.option, "s") == 0) || (strcmp(optionArgumentPair.option, "seed") == 0))
            {
                /* Seed */
                if(optionArgumentPair.argument == NULL)
                    seed = true;
            }
            else if((strcmp(optionArgumentPair.option, "S") == 0) || (strcmp(optionArgumentPair.option, "signed") == 0))
            {
                /* Random Signed 32-Bit Integer */
                if(optionArgumentPair.argument == NULL)
                    signedInteger = true;
            }
            else if((strcmp(optionArgumentPair.option, "u") == 0) || (strcmp(optionArgumentPair.option, "unsigned") == 0))
            {
                /* Random Unsigned 32-Bit Integer */
                if(optionArgumentPair.argument == NULL)
                    unsignedInteger = true;
            }
        }
    }

    /*** Handle Arguments ***/
    /* Help */
    if(help)
    {
        cliCallback_printfCallback(false, "usage: %s [OPTION]\n", CLI_COMMAND_HANDLER_CALLBACK_RANDOM_COMMAND_NAME);
        cliCallback_printfCallback(false, "  -c[COUNT], --count=[COUNT] (Set Random 32-Bit Integer Count)\n");
        cliCallback_printfCallback(false, "  -h, --help (Help)\n");
        cliCallback_printfCallback(false, "  -s, --seed (Get Seed)\n");
        cliCallback_printfCallback(false, "  -S, --signed (Get Random Signed 32-Bit Integer)\n");
        cliCallback_printfCallback(true, "  -u, --unsigned (Get Random Unsigned 32-Bit Integer)\n"); // Flush
    }

    /* Seed */
    if(seed)
    	cliCallback_printfCallback(true, "Seed: %u\n", randomCallback_getSeed()); // Flush

    /* Random Signed 32-Bit Integer */
    if(signedInteger)
    {
        cliCallback_printfCallback(false, "Random Signed 32-Bit Integer(s):\n");
        for(i = 0; i < count; i++)
        {
#ifdef __UNIT_TEST__
        	if(((i + 1) % 10) == 0)
        		cliCallback_printfCallback(true, "%u: %d\n", i + 1, random_getRandomSigned32BitInteger()); // Flush
        	else
        		cliCallback_printfCallback(false, "%u: %d\n", i + 1, random_getRandomSigned32BitInteger());
#else
        	if(((i + 1) % 10) == 0)
        		cliCallback_printfCallback(true, "%lu: %ld\n", i + 1, random_getRandomSigned32BitInteger()); // Flush
        	else
        		cliCallback_printfCallback(false, "%lu: %ld\n", i + 1, random_getRandomSigned32BitInteger());
#endif
        }
        cliCallback_printfCallback(true, ""); // Flush
    }

    /* Random Unsigned 32-Bit Integer */
    if(unsignedInteger)
    {
        cliCallback_printfCallback(false, "Random Unsigned 32-Bit Integer(s):\n");
        for(i = 0; i < count; i++)
        {
        	if(((i + 1) % 10) == 0)
        		cliCallback_printfCallback(true, "%lu: %lu\n", i + 1, random_getRandomUnsigned32BitInteger()); // Flush
        	else
        		cliCallback_printfCallback(false, "%lu: %lu\n", i + 1, random_getRandomUnsigned32BitInteger());
        }
        cliCallback_printfCallback(true, ""); // Flush
    }

	return STATUS_SUCCESS;
}

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
