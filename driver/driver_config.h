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
#include "led_driver.h"
#include "msp430fr6989.h"
#include "timer_a.h"

/****************************************************************************************************
 * Defines
 ****************************************************************************************************/

/*** Button ***/
/* GPIO */
#define DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PORT (GPIO_PORT_P1)
#define DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN  (GPIO_PIN1)
#define DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PORT (GPIO_PORT_P1)
#define DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PIN  (GPIO_PIN2)

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

/*** LED ***/
/* GPIO */
#define DRIVER_CONFIG_LED_GPIO_LED_1_PORT (GPIO_PORT_P1)
#define DRIVER_CONFIG_LED_GPIO_LED_1_PIN  (GPIO_PIN0)
#define DRIVER_CONFIG_LED_GPIO_LED_2_PORT (GPIO_PORT_P9)
#define DRIVER_CONFIG_LED_GPIO_LED_2_PIN  (GPIO_PIN7)

/* Timer */
#define DRIVER_CONFIG_LED_TIMER           (TIMER_A0_BASE)
#define DRIVER_CONFIG_LED_TIMER_LED_1_CCR (TIMER_A_CAPTURECOMPARE_REGISTER_1)
#define DRIVER_CONFIG_LED_TIMER_LED_2_CCR (TIMER_A_CAPTURECOMPARE_REGISTER_2)

#endif
