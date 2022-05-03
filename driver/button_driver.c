/****************************************************************************************************
 * FILE:    button_driver.c
 * BRIEF:   Button Driver Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "button_driver.h"
#include "driver_config.h"
#include "gpio.h"
#include "led_driver.h"
#include "msp430fr5xx_6xxgeneric.h"
#include "project.h"
#include <stdbool.h>
#include <stdint.h>

/****************************************************************************************************
 * Constants and Variables
 ****************************************************************************************************/

PROJECT_STATIC volatile uint8_t buttonDriver_state;

/****************************************************************************************************
 * Function Definitions (Public)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   buttonDriver_init
 * BRIEF:   Initialize
 * RETURN:  Returns Nothing
 * ARG:     No Arguments
 ****************************************************************************************************/
void buttonDriver_init(void)
{
    /*** Initialize ***/
    /* GPIO Pins */
    GPIO_setAsInputPinWithPullUpResistor(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PORT, DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN);
    GPIO_setAsInputPinWithPullUpResistor(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PORT, DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PIN);

    /* Interrupt (Button 1, Active Low) */
    GPIO_selectInterruptEdge(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PORT, DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_clearInterrupt(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PORT, DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN);
    GPIO_enableInterrupt(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PORT, DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN);

    /* Interrupt (Button 2, Active Low) */
    GPIO_selectInterruptEdge(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PORT, DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_clearInterrupt(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PORT, DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PIN);
    GPIO_enableInterrupt(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PORT, DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PIN);

    /* Button State */
    buttonDriver_state = 0x00;
}

/****************************************************************************************************
 * FUNCT:   buttonDriver_portInterruptHandler
 * BRIEF:   Port Interrupt Handler
 * RETURN:  Returns Nothing
 * ARG:     InterruptFlag: Interrupt Flag
 ****************************************************************************************************/
void buttonDriver_portInterruptHandler(uint16_t InterruptFlag)
{
    /*** Handle Interrupt */
    switch(InterruptFlag)
    {
        case P1IV_NONE:
            /*** No Interrupt ***/
            break;
        case P1IV_P1IFG0:
            /*** P1.0 Interrupt ***/
            break;
        case P1IV_P1IFG1:
            /*** BUTTON 1 (P1.1) Interrupt ***/
            /* Change Button State */
            buttonDriver_state ^= BUTTON_DRIVER_BUTTON_1;

            /* Switch Edge Direction */
            if((buttonDriver_state & BUTTON_DRIVER_BUTTON_1) == BUTTON_DRIVER_BUTTON_1)
            {
                /* Button Is Pressed; Button Edge Was High To Low; Switch To Low To High To Detect Button Release (Active Low) */
                GPIO_selectInterruptEdge(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PORT, DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN, GPIO_LOW_TO_HIGH_TRANSITION);
            }
            else
            {
                /* Button Is Released;Button Edge Was Low To High; Switch To High To Low To Detect Button Press (Active Low) */
                GPIO_selectInterruptEdge(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PORT, DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
            }
            break;
        case P1IV_P1IFG2:
            /*** BUTTON 2 (P1.2) Interrupt ***/
            /* Change Button State */
            buttonDriver_state ^= BUTTON_DRIVER_BUTTON_2;

            /* Switch Edge Direction */
            if((buttonDriver_state & BUTTON_DRIVER_BUTTON_2) == BUTTON_DRIVER_BUTTON_2)
            {
                /* Button Is Pressed; Button Edge Was High To Low; Switch To Low To High To Detect Button Release (Active Low) */
                GPIO_selectInterruptEdge(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PORT, DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PIN, GPIO_LOW_TO_HIGH_TRANSITION);
            }
            else
            {
                /* Button Is Released;Button Edge Was Low To High; Switch To High To Low To Detect Button Press (Active Low) */
                GPIO_selectInterruptEdge(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PORT, DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
            }
            break;
        case P1IV_P1IFG3:
            /*** P1.3 Interrupt ***/
            break;
        case P1IV_P1IFG4:
            /*** P1.4 Interrupt ***/
            break;
        case P1IV_P1IFG5:
            /*** P1.5 Interrupt ***/
            break;
        case P1IV_P1IFG6:
            /*** P1.6 Interrupt ***/
            break;
        case P1IV_P1IFG7:
            /*** P1.7 Interrupt ***/
            break;
        default:
            /*** Do Nothing */
            break;
    }
}

/****************************************************************************************************
 * FUNCT:   buttonDriver_tick
 * BRIEF:   Tick
 * RETURN:  void: Returns Nothing
 * ARG:     void: No Arguments
 ****************************************************************************************************/
void buttonDriver_tick(void)
{
	/*** Handle Button Press ***/
	/* Button 1 */
	if(!ledDriver_isEnabled(LED_DRIVER_LED_1))
	{
		if((buttonDriver_state & BUTTON_DRIVER_BUTTON_1) == BUTTON_DRIVER_BUTTON_1)
			ledDriver_setState(LED_DRIVER_LED_1, true);
		else
			ledDriver_setState(LED_DRIVER_LED_1, false);
	}

	/* Button 2 */
	if(!ledDriver_isEnabled(LED_DRIVER_LED_2))
	{
		if((buttonDriver_state & BUTTON_DRIVER_BUTTON_2) == BUTTON_DRIVER_BUTTON_2)
			ledDriver_setState(LED_DRIVER_LED_2, true);
		else
			ledDriver_setState(LED_DRIVER_LED_2, false);
	}
}
