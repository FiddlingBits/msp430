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
#include "msp430fr6989.h"

/****************************************************************************************************
 * Function Definitions (Public)
 ****************************************************************************************************/

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
    cliCallback_interruptHandler(__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG));
}
