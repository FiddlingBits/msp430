/****************************************************************************************************
 * FILE:    system.c
 * BRIEF:   System Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "cli.h"
#include "cli_callback.h"
#include "cli_command_handler_callback.h"
#include "cs.h"
#include "driver_config.h"
#include "gpio.h"
#include "msp430fr6989.h"
#include "pmm.h"
#include <stdint.h>
#include "system.h"
#include "wdt_a.h"

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

static void system_initApplication(void);
static void system_initClocks(void);

/****************************************************************************************************
 * Function Definitions (Public)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   system_init
 * BRIEF:   Initialize
 * RETURN:  Returns Nothing
 * ARG:     No Arguments
 ****************************************************************************************************/
void system_init(void)
{
    /*** Disable Interrupts ***/
    __disable_interrupt();

    /*** Disable/Hold Watchdog Timer ***/
    WDT_A_hold(WDT_A_BASE);

    /*** Disable GPIO Power-On Default High-Impedance Mode ***/
    PMM_unlockLPM5();

    /*** Initialize Clocks ***/
    system_initClocks();

    /*** Initialize Application ***/
    system_initApplication();

    /*** Enable Interrupts ***/
    __enable_interrupt();
}

/****************************************************************************************************
 * FUNCT:   system_reset
 * BRIEF:   Reset
 * RETURN:  Returns Nothing
 * ARG:     No Arguments
 ****************************************************************************************************/
void system_reset(void)
{
    /*** Watchdog Timer Password Violation Generates A Power-Up Clear (PUC) Reset ***/
    HWREG16(WDT_A_BASE + OFS_WDTCTL) = (uint16_t)(~WDTPW); // If WDTPW Isn't Written A Power Up Clear (PUC) Is Generated

    /*** Application Only ***/
#ifndef __UNIT_TEST__
    while(1)
        continue;
#endif
}

/****************************************************************************************************
 * Function Definitions (Private)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   system_initApplication
 * BRIEF:   Initialize Application
 * RETURN:  Returns Nothing
 * ARG:     No Arguments
 ****************************************************************************************************/
static void system_initApplication(void)
{
    /*** First Stage Initialization: No Dependencies ***/
    cliCallback_init();

    /*** Second Stage Initialization: Dependent On First Stage ***/
    (void)cli_init(cliCallback_alertProcessInputCallback, cliCallback_printfCallback);

    /*** Third Stage Initialization: Dependent On Second Stage ***/
    cliCommandHandlerCallback_init();
}

/****************************************************************************************************
 * FUNCT:   system_initClocks
 * BRIEF:   Initialize Clocks
 * RETURN:  Returns Nothing
 * ARG:     No Arguments
 ****************************************************************************************************/
static void system_initClocks(void)
{
    /*** Initialize Clocks ***/
    /* Frequencies */
    CS_setExternalClockSource(32768, 0); // 0 (No High-Frequency Crystal Oscillator (HFXT))

    /* Low-Frequency Crystal Oscillator (LFXT) */
    GPIO_setAsPeripheralModuleFunctionInputPin(DRIVER_CONFIG_LFXT_GPIO_IN_PORT, DRIVER_CONFIG_LFXT_GPIO_IN_PIN, DRIVER_CONFIG_LFXT_GPIO_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(DRIVER_CONFIG_LFXT_GPIO_OUT_PORT, DRIVER_CONFIG_LFXT_GPIO_OUT_PIN, DRIVER_CONFIG_LFXT_GPIO_FUNCTION);
    CS_turnOnLFXT(CS_LFXT_DRIVE_3); // Maximum Drive Strength

    /* System Clocks */
    CS_setDCOFreq(CS_DCORSEL_0, CS_DCOFSEL_6); // Digitally Controlled Oscillator (DCO): 8 MHz
    CS_initClockSignal(CS_ACLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1); // Auxiliary Clock (ACLK): 32768 Hz / 1 = 32768 Hz
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1); // Master Clock (MCLK): 8 MHz / 1 = 8 MHz
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1); // Sub-Main CLock (SMCLK): 8 MHz / 1 = 8 MHz
}
