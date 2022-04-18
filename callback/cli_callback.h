/****************************************************************************************************
 * FILE:    cli_callback.h
 * BRIEF:   Command Line Interface (CLI) Callback Header File
 ****************************************************************************************************/

#ifndef CLI_CALLBACK_H
#define CLI_CALLBACK_H

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include <stdbool.h>
#include <stdint.h>

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

extern void cliCallback_alertProcessInputCallback(void);
extern void cliCallback_init(void);
extern void cliCallback_interruptHandler(uint16_t InterruptFlag);
extern void cliCallback_printfCallback(const bool Flush, const char * const Format, ...);
extern void cliCallback_tick(void);

#endif
