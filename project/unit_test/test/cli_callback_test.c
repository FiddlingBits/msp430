/****************************************************************************************************
 * FILE:    cli_callback_test.c
 * BRIEF:   Command Line Interface (CLI) Callback Test Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "cli.h"
#include "cli_callback.h"
#include "cli_callback_test.h"
#include "driver_config.h"
#include "eusci_a_uart.h"
#include "gpio.h"
#include "hw_memmap.h"
#include "msp430fr5xx_6xxgeneric.h"
#include "status.h"
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "system.h"
#include "unity_fixture.h"

/****************************************************************************************************
 * Constants and Variables
 ****************************************************************************************************/

static char cliCallbackTest_printfOutput[CLI_CALLBACK_TEST_PRINTF_OUTPUT_LENGTH];

/****************************************************************************************************
 * Function Definitions
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   cliCallbackTest_copyPrintfOutput
 * BRIEF:   Copy Printf Output
 * RETURN:  Returns Nothing
 * ARG:     Output: Printf Output
 ****************************************************************************************************/
void cliCallbackTest_copyPrintfOutput(const char * const Output)
{
    /*** Copy Printf Output ***/
    (void)strcat(cliCallbackTest_printfOutput, Output);
}

/****************************************************************************************************
 * FUNCT:   cliCallbackTest_getPrintfOutputCopy
 * BRIEF:   Get Printf Output Copy
 * RETURN:  Returns Nothing
 * ARG:     copy: Printf Output Copy
 ****************************************************************************************************/
void cliCallbackTest_getPrintfOutputCopy(char * const copy)
{
    /*** Get Printf Output Copy ***/
    (void)strncpy(copy, cliCallbackTest_printfOutput, CLI_CALLBACK_TEST_PRINTF_OUTPUT_LENGTH);
    cliCallbackTest_printfOutput[0] = '\0';
}

/****************************************************************************************************
 * FUNCT:   cliCallbackTest_init
 * BRIEF:   Initialize
 * RETURN:  Returns Nothing
 * ARG:     No Arguments
 ****************************************************************************************************/
void cliCallbackTest_init(void)
{
    /*** Initialize ***/
    cliCallbackTest_printfOutput[0] = '\0';
}

/****************************************************************************************************
 * FUNCT:   cliCallbackTest_sendCommand
 * BRIEF:   Send Command
 * RETURN:  Returns Nothing
 * ARG:     Command: Command
 ****************************************************************************************************/
void cliCallbackTest_sendCommand(const char * const Command)
{
    size_t i;
    
    /*** Send Command ***/
    /* Receive Character */
    for(i = 0; i < strlen(Command); i++)
        TEST_ASSERT_EQUAL_INT(STATUS_SUCCESS, cli_addReceiveCharacter(Command[i]));
    
    /* Process Input */
    TEST_ASSERT_EQUAL_INT(STATUS_SUCCESS, cli_processInput());
}

/****************************************************************************************************
 * Test Group
 ****************************************************************************************************/

TEST_GROUP(cli_callback_test);

/****************************************************************************************************
 * Test Setup And Teardown
 ****************************************************************************************************/

TEST_SETUP(cli_callback_test)
{
    /*** Set Up ***/
    /* Simulation */
    hwMemmap_init();
    
    /* Application */
    system_init();
    
    /* Test */
    cliCallbackTest_init();
}

TEST_TEAR_DOWN(cli_callback_test)
{
}

/****************************************************************************************************
 * Tests
 ****************************************************************************************************/

TEST(cli_callback_test, init)
{
    uint16_t baseAddress;
    
    /********************************************************************************
     * Test 1: Initialize
     ********************************************************************************/
    
    /*** Subtest 1: GPIO Pins ***/
    /* Rx */
    baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_CLI_GPIO_RX_PORT];
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PADIR) & DRIVER_CONFIG_CLI_GPIO_RX_PIN); // Input: PxDIR = 0b
    TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_CLI_GPIO_RX_PIN, HWREG16(baseAddress + OFS_PASEL0) & DRIVER_CONFIG_CLI_GPIO_RX_PIN); // Primary Module Function: PxSEL0 = 1b
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PASEL1) & DRIVER_CONFIG_CLI_GPIO_RX_PIN); // Primary Module Function: PxSEL1 = 0b
    
    /* Tx */
    baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_CLI_GPIO_RX_PORT];
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PADIR) & DRIVER_CONFIG_CLI_GPIO_TX_PIN); // Input: PxDIR = 0b
    TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_CLI_GPIO_TX_PIN, HWREG16(baseAddress + OFS_PASEL0) & DRIVER_CONFIG_CLI_GPIO_TX_PIN); // Primary Module Function: PxSEL0 = 1b
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PASEL1) & DRIVER_CONFIG_CLI_GPIO_TX_PIN); // Primary Module Function: PxSEL1 = 0b
    
    /*** Subtest 2: UART ***/
    /* Initialized */
    TEST_ASSERT_EQUAL_HEX16(EUSCI_A_UART_CLOCKSOURCE_SMCLK, HWREG16(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS + OFS_UCAxCTLW0) & UCSSEL_3); // Clock Source: Sub-Main Clock: UCAxCTLW0.UCSSELx = 11b
    TEST_ASSERT_EQUAL_HEX16(EUSCI_A_UART_LSB_FIRST, HWREG16(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS + OFS_UCAxCTLW0) & UCMSB); // Least Significant Bit (LSB) First: UCAxCTLW0.UCMSB = 0b
    TEST_ASSERT_EQUAL_HEX16(EUSCI_A_UART_ONE_STOP_BIT, HWREG16(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS + OFS_UCAxCTLW0) & UCSPB); // 1 Stop Bit: UCAxCTLW0.UCSPB = 0b
    TEST_ASSERT_EQUAL_HEX16(EUSCI_A_UART_NO_PARITY, HWREG16(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS + OFS_UCAxCTLW0) & UCPEN); // No Parity: UCAxCTLW0.UCPEN = 0b
    TEST_ASSERT_EQUAL_HEX16(4, HWREG16(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS + OFS_UCAxBRW)); // Clock Prescaler
    TEST_ASSERT_EQUAL_HEX16((85 << 8) | (5 << 4) | EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION, HWREG16(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS + OFS_UCAxMCTLW)); // Modulation Register
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS + OFS_UCAxCTLW0) & UCSYNC); // Asynchronous: UCAxCTLW0.UCSYNC = 0b
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS + OFS_UCAxCTLW0) & UC7BIT);// 8-Bit Data: UCAxCTLW0.UC7BIT = 0b
    TEST_ASSERT_EQUAL_HEX16(EUSCI_A_UART_MODE, HWREG16(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS + OFS_UCAxCTLW0) & UCMODE_3); // UART Mode: UCAxCTLW0.UCMODEx = 00b
    
    /* Enabled */
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS + OFS_UCAxCTLW0) & UCSWRST); // UCAxCTLW0.UCSWRST = 0b
    
    /*** Subtest 3: Interrupts ***/
    TEST_ASSERT_EQUAL_HEX16(EUSCI_A_UART_RECEIVE_INTERRUPT, HWREG16(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS + OFS_UCAxIE) & EUSCI_A_UART_RECEIVE_INTERRUPT); // Rx
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS + OFS_UCAxIE) & EUSCI_A_UART_TRANSMIT_INTERRUPT); // Tx
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS + OFS_UCAxIE) & EUSCI_A_UART_STARTBIT_INTERRUPT); // Start Bit
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS + OFS_UCAxIE) & EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT); // Transmit Complete
}

TEST(cli_callback_test, printfCallback)
{
    const char *expectedOutput;
    char actualOutput[CLI_CALLBACK_TEST_PRINTF_OUTPUT_LENGTH];
    
    /********************************************************************************
     * Test 1: DMA
     ********************************************************************************/
    
    /*** Set Up ***/
    cliCallback_printfCallback(true, "Test"); // Flush
    
    /*** Subtest 1: Initialized ***/
    TEST_ASSERT_EQUAL_HEX16(DMA_TRANSFER_BLOCK, HWREG16(DMA_BASE + DRIVER_CONFIG_CLI_DMA_CHANNEL + OFS_DMA0CTL) & DMADT); // Transfer Mode: Block: DMA0CTL.DMADT = 001b
    TEST_ASSERT_EQUAL_HEX16(DMA_SIZE_SRCBYTE_DSTBYTE & DMADSTBYTE, HWREG16(DMA_BASE + DRIVER_CONFIG_CLI_DMA_CHANNEL + OFS_DMA0CTL) & DMADSTBYTE); // Destination Data Size: Byte: DMA0CTL.DMADSTBYTE = 1b
    TEST_ASSERT_EQUAL_HEX16(DMA_SIZE_SRCBYTE_DSTBYTE & DMASRCBYTE, HWREG16(DMA_BASE + DRIVER_CONFIG_CLI_DMA_CHANNEL + OFS_DMA0CTL) & DMASRCBYTE); // Source Data Size: Byte: DMA0CTL.DMASRCBYTE = 1b
    TEST_ASSERT_EQUAL_HEX16(DMA_TRIGGER_HIGH, HWREG16(DMA_BASE + DRIVER_CONFIG_CLI_DMA_CHANNEL + OFS_DMA0CTL) & DMALEVEL); // Trigger Type/Level: High: DMA0CTL.DMALEVEL = 1b
    TEST_ASSERT_EQUAL_HEX16(strlen("Test"), HWREG16(DMA_BASE + DRIVER_CONFIG_CLI_DMA_CHANNEL + OFS_DMA0SZ)); // Transfer Size
    TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_CLI_DMA_TRIGGER_SOURCE, HWREG16(DMA_BASE + DRIVER_CONFIG_CLI_DMA_CHANNEL + OFS_DMACTL0) & DMA0TSEL);
    
    /*** Subtest 2: Source Address ***/
    TEST_ASSERT_EQUAL_HEX16(DMA_DIRECTION_INCREMENT, HWREG16(DMA_BASE + DRIVER_CONFIG_CLI_DMA_CHANNEL + OFS_DMA0CTL) & DMASRCINCR_3);
    
    /*** Subtest 3: Destination Address ***/
    TEST_ASSERT_EQUAL_HEX16(EUSCI_A_UART_getTransmitBufferAddress(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS), HWREG16(DMA_BASE + DRIVER_CONFIG_CLI_DMA_CHANNEL + OFS_DMA0DA));
    TEST_ASSERT_EQUAL_HEX16(DMA_DIRECTION_UNCHANGED, HWREG16(DMA_BASE + DRIVER_CONFIG_CLI_DMA_CHANNEL + OFS_DMA0CTL) & DMADSTINCR_3);
    
    /*** Transfer Started ***/
    TEST_ASSERT_EQUAL_HEX16(DMAREQ, HWREG16(DMA_BASE + DRIVER_CONFIG_CLI_DMA_CHANNEL + OFS_DMA0CTL) & DMAREQ);
    
    /*** Clean Up ***/
    cliCallbackTest_getPrintfOutputCopy(actualOutput);
    TEST_ASSERT_EQUAL_STRING("Test", actualOutput);
    
    /********************************************************************************
     * Test 2: Output
     ********************************************************************************/
    
    /*** Subtest 1: Empty String ***/
    /* Set Up */
    expectedOutput = "";
    
    /* Printf */
    cliCallback_printfCallback(true, ""); // Flush
    
    /* Output As Expected */
    cliCallbackTest_getPrintfOutputCopy(actualOutput);
    TEST_ASSERT_EQUAL_STRING(expectedOutput, actualOutput);
    
    /*** Subtest 2: String Without Arguments ***/
    /* Set Up */
    expectedOutput = "String Without Arguments\n";
    
    /* Printf */
    cliCallback_printfCallback(true, "String Without Arguments\n"); // Flush
    
    /* Output As Expected */
    cliCallbackTest_getPrintfOutputCopy(actualOutput);
    TEST_ASSERT_EQUAL_STRING(expectedOutput, actualOutput);
    
    /*** Subtest 3: String With Arguments ***/
    /* Set Up */
    expectedOutput = "String With 3.0 Arguments\n";
    
    /* Printf */
    cliCallback_printfCallback(true, "String %s %0.1f Argume%cts\n", "With", 3.0, 'n'); // Flush
    
    /* Output As Expected */
    cliCallbackTest_getPrintfOutputCopy(actualOutput);
    TEST_ASSERT_EQUAL_STRING(expectedOutput, actualOutput);
}

/****************************************************************************************************
 * Test Group Runner
 ****************************************************************************************************/

TEST_GROUP_RUNNER(cli_callback_test)
{
    RUN_TEST_CASE(cli_callback_test, init)
    RUN_TEST_CASE(cli_callback_test, printfCallback)
}
