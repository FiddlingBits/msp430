/****************************************************************************************************
 * FILE:    interrupt_handler.h
 * BRIEF:   Interrupt Handler Header File
 ****************************************************************************************************/

#ifndef INTERRUPT_HANDLER_H
#define INTERRUPT_HANDLER_H

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

extern __interrupt void interruptHandler_port1Vector(void);
extern __interrupt void interruptHandler_timer0A1Vector(void);
extern __interrupt void interruptHandler_usciA1Vector(void);

#endif
