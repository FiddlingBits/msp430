/****************************************************************************************************
 * FILE:    led_driver.h
 * BRIEF:   LED (Light-Emitting Diode) Driver Header File
 ****************************************************************************************************/

#ifndef LED_DRIVER_H
#define LED_DRIVER_H

/****************************************************************************************************
 * Defines
 ****************************************************************************************************/

#define LED_DRIVER_CONVERT_MILLISECONDS_TO_TICKS(MILLISECONDS) (uint16_t)(((uint32_t)65536 * MILLISECONDS) / 2000)

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include <stdbool.h>
#include <stdint.h>

/****************************************************************************************************
 * Type Definitions
 ****************************************************************************************************/

typedef enum ledDriver_led_e
{
    LED_DRIVER_LED_1,
    LED_DRIVER_LED_2,
    LED_DRIVER_LED_COUNT
} ledDriver_led_t;

typedef struct ledDriver_ledBlink_s
{
    bool on;
    uint16_t onMilliseconds;
    uint16_t offMilliseconds;
} ledDriver_ledBlink_t;

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

extern bool ledDriver_enableBlink(const ledDriver_led_t Led, const uint16_t OnMilliseconds, const uint16_t OffMilliseconds);
extern void ledDriver_init(void);
extern void ledDriver_timerInterruptHandler(uint16_t InterruptFlag);

#endif
