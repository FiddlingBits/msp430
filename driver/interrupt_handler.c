/****************************************************************************************************
 * FILE:    interrupt_handler.c
 * BRIEF:   Interrupt Handler Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "cli_callback.h"
#include "interrupt_handler.h"
#include "intrinsics.h"
#include "led_driver.h"
#include "msp430fr6989.h"

/****************************************************************************************************
 * Function Definitions (Public)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   interruptHandler_timer0A1Vector
 * BRIEF:   Timer A0, Capture/Compare Register 1-4 Vector Interrupt Handler
 * RETURN:  Returns Nothing
 * ARG:     No Arguments
 ****************************************************************************************************/
#pragma vector=TIMER0_A1_VECTOR // Timer A0, Capture/Compare Register 1-4; Timer A0, Capture/Compare Register 0 Has It's Own Interrupt Vector
__interrupt void interruptHandler_timer0A1Vector(void)
{
    /*** Handle Interrupt ***/
    ledDriver_timerInterruptHandler(__even_in_range(TA0IV, TA0IV_TAIFG));
}

/****************************************************************************************************
 * FUNCT:   interruptHandler_usciA1Vector
 * BRIEF:   Universal Serial Communications Interface (USCI) A1 Vector Interrupt Handler
 * RETURN:  Returns Nothing
 * ARG:     No Arguments
 ****************************************************************************************************/
#pragma vector=USCI_A1_VECTOR
__interrupt void interruptHandler_usciA1Vector(void)
{
    /*** Handle Interrupt ***/
    cliCallback_usciInterruptHandler(__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG));
}
