/****************************************************************************************************
 * FILE:    random_callback.c
 * BRIEF:   Random Callback Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "project.h"
#include "random_callback.h"
#include <stdint.h>
#include <stdlib.h>
#include "tlv.h"

/****************************************************************************************************
 * Constants and Variables
 ****************************************************************************************************/

#pragma PERSISTENT(randomCallback_seed)
PROJECT_STATIC uint16_t randomCallback_seed = 0;

/****************************************************************************************************
 * Function Definitions (Public)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   randomCallback_getRandomUnsigned32BitIntegerCallback
 * BRIEF:   Get Random Unsigned 32-Bit Integer Callback
 * RETURN:  uint32_t: Random Unsigned 32-Bit Integer
 * ARG:     void: No Arguments
 * NOTE:    Unsigned 32-Bit Integer Range: 0 To 4,294,967,295 (0x00000000 To 0xFFFFFFFF)
 ****************************************************************************************************/
uint32_t randomCallback_getRandomUnsigned32BitIntegerCallback(void)
{
    /*** Get Random Unsigned 32-Bit Integer ***/
#ifdef __UNIT_TEST_
	return (uint32_t)(((rand() & 0xFFFF) << 16) | (rand() & 0xFFFF)); // Returns Value Between 0 And RAND_MAX (2,147,483,647 For Unit Test)
#else
	return ((((uint32_t)rand() & 0xFF) << 24) | (((uint32_t)rand() & 0xFF) << 16) | (((uint32_t)rand() & 0xFF) << 8) | ((uint32_t)rand() & 0xFF)); // Returns Value Between 0 And RAND_MAX (32,767 For MSP430)
#endif
}

/****************************************************************************************************
 * FUNCT:   randomCallback_getSeed
 * BRIEF:   Get Seed
 * RETURN:  uint16_t: Seed
 * ARG:     void: No Arguments
 ****************************************************************************************************/
uint16_t randomCallback_getSeed(void)
{
	/*** Get Seed ***/
	return randomCallback_seed - 1; // Was Incremented In randomCallback_init
}

/****************************************************************************************************
 * FUNCT:   randomCallback_init
 * BRIEF:   Initialize
 * RETURN:  void: Returns Nothing
 * ARG:     void: No Arguments
 ****************************************************************************************************/
void randomCallback_init(void)
{
	/*** Initialize ***/
	if(randomCallback_seed == 0)
		randomCallback_seed = TLV_getDeviceType(); // Unique Device ID
	srand(randomCallback_seed++);
}
