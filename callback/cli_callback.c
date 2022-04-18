/****************************************************************************************************
 * FILE:    cli_callback.c
 * BRIEF:   Command Line Interface (CLI) Callback Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Defines
 ****************************************************************************************************/

#define CLI_CALLBACK_PRINTF_CALLBACK_STRING_LENGTH (256)

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

/*** Common ***/
#include "cli.h"
#include "cli_callback.h"
#include "driver_config.h"
#include "eusci_a_uart.h"
#include "dma.h"
#include "gpio.h"
#include "msp430fr5xx_6xxgeneric.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/*** Unit Test ***/
#ifdef __UNIT_TEST__
  #include "cli_callback_test_helper.h"
#endif

/****************************************************************************************************
 * Constants and Variables
 ****************************************************************************************************/

static volatile uint8_t cliCallback_alertProcessInputCount;

/****************************************************************************************************
 * Function Definitions (Public)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   cliCallback_alertProcessInputCallback
 * BRIEF:   Alert Process Input Callback
 * RETURN:  void: Returns Nothing
 * ARG:     void: No Arguments
 ****************************************************************************************************/
void cliCallback_alertProcessInputCallback(void)
{
    /*** Increment Alert Process Input Count ***/
    cliCallback_alertProcessInputCount++;
}

/****************************************************************************************************
 * FUNCT:   cliCallback_init
 * BRIEF:   Initialize
 * RETURN:  void: Returns Nothing
 * ARG:     void: No Arguments
 ****************************************************************************************************/
void cliCallback_init(void)
{
    EUSCI_A_UART_initParam uartInit;

    /*** Deinitialize (May Have Dirty Settings After Reset) ***/
    DMA_disableTransfers(DRIVER_CONFIG_CLI_DMA_CHANNEL);

    /*** Initialize ***/
    /* GPIO */
    GPIO_setAsPeripheralModuleFunctionInputPin(DRIVER_CONFIG_CLI_GPIO_RX_PORT, DRIVER_CONFIG_CLI_GPIO_RX_PIN, DRIVER_CONFIG_CLI_GPIO_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(DRIVER_CONFIG_CLI_GPIO_TX_PORT, DRIVER_CONFIG_CLI_GPIO_TX_PIN, DRIVER_CONFIG_CLI_GPIO_FUNCTION);

    /* UART: 115200 8-N-1 ; See: http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html And MSP430FR6xx User's Guide: 30.3.10 Setting A Baud Rate */
    uartInit.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    uartInit.clockPrescalar = 4;
    uartInit.firstModReg = 5;
    uartInit.secondModReg = 85;
    uartInit.parity = EUSCI_A_UART_NO_PARITY;
    uartInit.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    uartInit.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    uartInit.uartMode = EUSCI_A_UART_MODE;
    uartInit.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;
    EUSCI_A_UART_init(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS, &uartInit);
    EUSCI_A_UART_enable(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS);

    /* UART Interrupt */
    EUSCI_A_UART_clearInterrupt(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS, EUSCI_A_UART_RECEIVE_INTERRUPT);
    EUSCI_A_UART_enableInterrupt(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS, EUSCI_A_UART_RECEIVE_INTERRUPT);

    /* Alert Process Input Count */
    cliCallback_alertProcessInputCount = 0;
}

/****************************************************************************************************
 * FUNCT:   cliCallback_interruptHandler
 * BRIEF:   Interrupt Handler
 * RETURN:  Returns Nothing
 * ARG:     No Arguments
 ****************************************************************************************************/
void cliCallback_interruptHandler(uint16_t InterruptFlag)
{
    /*** Handle Interrupt */
    switch(InterruptFlag)
    {
        case USCI_NONE:
            /*** No Interrupt ***/
            break;
        case USCI_UART_UCRXIFG:
            /*** Receive Interrupt ***/
            (void)cli_addReceiveCharacter((char)EUSCI_A_UART_receiveData(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS));
            break;
        case USCI_UART_UCTXIFG:
            /*** Transmit Interrupt ***/
            break;
        case USCI_UART_UCSTTIFG:
            /*** Start Bit Interrupt ***/
            break;
        case USCI_UART_UCTXCPTIFG:
            /*** Transmit Complete Interrupt ***/
            break;
        default:
            /*** Do Nothing ***/
            break;
    }
}

/****************************************************************************************************
 * FUNCT:   cliCallback_printfCallback
 * BRIEF:   printf Callback
 * RETURN:  void: Returns Nothing
 * ARG:     Flush: Flush (true) Or Don't Flush (false)
 * ARG:     Format: Format String
 * ARG:     ...: Optional Arguments
 ****************************************************************************************************/
void cliCallback_printfCallback(const bool Flush, const char * const Format, ...)
{
    static char printfString[CLI_CALLBACK_PRINTF_CALLBACK_STRING_LENGTH];
    static size_t i = 0;
    DMA_initParam dmaInit;
    va_list arguments;

    /*** Unit Test Only ***/
#ifdef __UNIT_TEST__
    DMA_disableTransfers(DRIVER_CONFIG_CLI_DMA_CHANNEL); // DMA Is Disabled After Completion Of Block Transfer
#endif

    /*** Wait Until DMA Disabled (Automatically Happens On Completion Of Block Transfer) And UART NOT Busy ***/
    while(((HWREG16(DMA_BASE + DRIVER_CONFIG_CLI_DMA_CHANNEL + OFS_DMA0CTL) & DMAEN) == DMAEN) || ((HWREG16(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS + OFS_UCAxSTATW) & UCBUSY) == UCBUSY))
        continue;

    /*** Decode Format String And Optional Arguments And Write To Print String ***/
    va_start(arguments, Format);
    i += (size_t)vsnprintf(&printfString[i], sizeof(printfString) - i, Format, arguments);
    va_end(arguments);

    /*** Flush ***/
    if(Flush)
    {
        /* Initialize DMA */
        dmaInit.channelSelect = DRIVER_CONFIG_CLI_DMA_CHANNEL;
        dmaInit.transferModeSelect = DMA_TRANSFER_BLOCK;
        dmaInit.transferSize = (uint16_t)strlen(printfString);
        dmaInit.triggerSourceSelect = DRIVER_CONFIG_CLI_DMA_TRIGGER_SOURCE;
        dmaInit.transferUnitSelect = DMA_SIZE_SRCBYTE_DSTBYTE;
        dmaInit.triggerTypeSelect = DMA_TRIGGER_HIGH;
        DMA_init(&dmaInit);

        /* Set DMA Addresses And Start Transfer */
        DMA_setSrcAddress(DRIVER_CONFIG_CLI_DMA_CHANNEL, (uint32_t)printfString, DMA_DIRECTION_INCREMENT);
        DMA_setDstAddress(DRIVER_CONFIG_CLI_DMA_CHANNEL, EUSCI_A_UART_getTransmitBufferAddress(DRIVER_CONFIG_CLI_UART_BASE_ADDRESS), DMA_DIRECTION_UNCHANGED);
        DMA_enableTransfers(DRIVER_CONFIG_CLI_DMA_CHANNEL);
        DMA_startTransfer(DRIVER_CONFIG_CLI_DMA_CHANNEL); // DMA Is Disabled After Completion Of Block Transfer

        /* Reset Index */
        i = 0;

        /* Unit Test Only */
#ifdef __UNIT_TEST__
        cliCallbackTestHelper_copyPrintfOutput(printfString);
#endif
    }
}

/****************************************************************************************************
 * FUNCT:   cliCallback_tick
 * BRIEF:   Tick
 * RETURN:  void: Returns Nothing
 * ARG:     void: No Arguments
 ****************************************************************************************************/
void cliCallback_tick(void)
{
    /*** Alert Process Input***/
    while(cliCallback_alertProcessInputCount > 0)
    {
        /*** Process Input ***/
        (void)cli_processInput();
        cliCallback_alertProcessInputCount--;
    }
}
