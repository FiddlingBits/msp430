/****************************************************************************************************
 * FILE:    driver_config.h
 * BRIEF:   Driver Configuration Header File
 ****************************************************************************************************/

#ifndef DRIVER_CONFIG_H
#define DRIVER_CONFIG_H

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "dma.h"
#include "gpio.h"
#include "msp430fr6989.h"

/****************************************************************************************************
 * Defines
 ****************************************************************************************************/

/*** Command Line Interface (CLI) ***/
/* DMA */
#define DRIVER_CONFIG_CLI_DMA_CHANNEL        (DMA_CHANNEL_0)
#define DRIVER_CONFIG_CLI_DMA_TRIGGER_SOURCE (DMA_TRIGGERSOURCE_17) // See MSP430FR698x Datasheet: 6.11.8 DMA Controller

/* GPIO */
#define DRIVER_CONFIG_CLI_GPIO_FUNCTION (GPIO_PRIMARY_MODULE_FUNCTION)
#define DRIVER_CONFIG_CLI_GPIO_RX_PORT  (GPIO_PORT_P3)
#define DRIVER_CONFIG_CLI_GPIO_RX_PIN   (GPIO_PIN5)
#define DRIVER_CONFIG_CLI_GPIO_TX_PORT  (GPIO_PORT_P3)
#define DRIVER_CONFIG_CLI_GPIO_TX_PIN   (GPIO_PIN4)

/* UART */
#define DRIVER_CONFIG_CLI_UART_BASE_ADDRESS (EUSCI_A1_BASE)

/*** Low-Frequency Crystal Oscillator (LFXT) ***/
/* GPIO */
#define DRIVER_CONFIG_LFXT_GPIO_FUNCTION (GPIO_PRIMARY_MODULE_FUNCTION)
#define DRIVER_CONFIG_LFXT_GPIO_IN_PORT  (GPIO_PORT_PJ)
#define DRIVER_CONFIG_LFXT_GPIO_IN_PIN   (GPIO_PIN4)
#define DRIVER_CONFIG_LFXT_GPIO_OUT_PORT (GPIO_PORT_PJ)
#define DRIVER_CONFIG_LFXT_GPIO_OUT_PIN  (GPIO_PIN5)

#endif
