/****************************************************************************************************
 * FILE:    driver_config.h
 * BRIEF:   Driver Configuration Header File
 ****************************************************************************************************/

#ifndef DRIVER_CONFIG_H
#define DRIVER_CONFIG_H

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "gpio.h"

/****************************************************************************************************
 * Defines
 ****************************************************************************************************/

/*** Low-Frequency Crystal Oscillator (LFXT) ***/
/* GPIO */
#define DRIVER_CONFIG_LFXT_GPIO_IN_PORT  (GPIO_PORT_PJ)
#define DRIVER_CONFIG_LFXT_GPIO_IN_PIN   (GPIO_PIN4)
#define DRIVER_CONFIG_LFXT_GPIO_OUT_PORT (GPIO_PORT_PJ)
#define DRIVER_CONFIG_LFXT_GPIO_OUT_PIN  (GPIO_PIN5)

#endif
