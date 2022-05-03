/****************************************************************************************************
 * FILE:    led_driver.c
 * BRIEF:   LED (Light-Emitting Diode) Driver Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "driver_config.h"
#include "gpio.h"
#include "led_driver.h"
#include "msp430fr6989.h"
#include "project.h"
#include <stdbool.h>
#include <stdint.h>
#include "timer_a.h"

/****************************************************************************************************
 * Constants And Variables
 ****************************************************************************************************/

PROJECT_STATIC volatile ledDriver_ledBlink_t ledDriver_ledBlink[LED_DRIVER_LED_COUNT];

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

static void ledDriver_resetLedBlink(const ledDriver_led_t Led);

/****************************************************************************************************
 * Function Definitions (Public)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   ledDriver_enableBlink
 * BRIEF:   Enable Blink
 * RETURN:  bool: Success (true) Or Failure (false)
 * ARG:     Led: LED
 * ARG:     OnMilliseconds: On Milliseconds
 * ARG:     OffMilliseconds: Off Milliseconds
 * NOTE:    On And Off Milliseconds Must Be No Greater Than 2000 Milliseconds; Timer Clock Is 32768
 *          Hz (ACLK) / 1 = 32768 Hz; It Rolls Over At 65535 To 0, Every 2000 Milliseconds
 * NOTE:    A Non-Zero On Milliseconds And A Zero Off Milliseconds Will Result In Solid On LED
 * NOTE:    A Zero On Milliseconds And A Non-Zero Off Milliseconds Will Result In Solid Off LED
 * NOTE:	A Zero On And Off Milliseconds Will Disable Blink
 ****************************************************************************************************/
bool ledDriver_enableBlink(const ledDriver_led_t Led, const uint16_t OnMilliseconds, const uint16_t OffMilliseconds)
{
    uint16_t captureCompareRegister = 0;
    Timer_A_initCompareModeParam initCompareModeParam;

    /*** Error Check ***/
    if((OnMilliseconds > 2000) || (OffMilliseconds > 2000))
        return false;

    /*** Enable Blink ***/
    /* Set Up */
    switch(Led)
    {
        case LED_DRIVER_LED_1:
            captureCompareRegister = DRIVER_CONFIG_LED_TIMER_LED_1_CCR;
            break;
        case LED_DRIVER_LED_2:
            captureCompareRegister = DRIVER_CONFIG_LED_TIMER_LED_2_CCR;
            break;
        case LED_DRIVER_LED_COUNT:
        default:
        	/* Error */
            return false;
    }

    /* Clear And Disable Capture/Compare Interrupt */
    Timer_A_clearCaptureCompareInterrupt(DRIVER_CONFIG_LED_TIMER, captureCompareRegister);
    Timer_A_disableCaptureCompareInterrupt(DRIVER_CONFIG_LED_TIMER, captureCompareRegister);

    /* Reset Blink */
    ledDriver_resetLedBlink(Led);

    /* Enable Blink */
    if((OnMilliseconds > 0) && (OffMilliseconds == 0))
    {
        /* Solid On */
    	ledDriver_ledBlink[Led].enabled = true;
        ledDriver_setState(Led, true);
    }
    else if((OnMilliseconds == 0) && (OffMilliseconds > 0))
    {
        /* Solid Off */
    	ledDriver_ledBlink[Led].enabled = true;
        ledDriver_setState(Led, false);
    }
    else if((OnMilliseconds > 0) && (OffMilliseconds > 0))
    {
        /* Blink Settings And Pre-Set LED State To On */
    	ledDriver_ledBlink[Led].enabled = true;
        ledDriver_ledBlink[Led].on = true;
        ledDriver_ledBlink[Led].onMilliseconds = OnMilliseconds;
        ledDriver_ledBlink[Led].offMilliseconds = OffMilliseconds;
        ledDriver_setState(Led, true);

        /* Stop Timer */
        Timer_A_startCounter(DRIVER_CONFIG_LED_TIMER, TIMER_A_STOP_MODE);

        /* Configure Timer Capture/Compare */
        initCompareModeParam.compareRegister = captureCompareRegister;
        initCompareModeParam.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
        initCompareModeParam.compareOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;
        initCompareModeParam.compareValue = Timer_A_getCaptureCompareCount(DRIVER_CONFIG_LED_TIMER, captureCompareRegister) + LED_DRIVER_CONVERT_MILLISECONDS_TO_TICKS(ledDriver_ledBlink[Led].onMilliseconds);
        Timer_A_initCompareMode(DRIVER_CONFIG_LED_TIMER, &initCompareModeParam);

        /* Restart Timer */
        Timer_A_startCounter(DRIVER_CONFIG_LED_TIMER, TIMER_A_CONTINUOUS_MODE);
    }

    return true;
}

/****************************************************************************************************
 * FUNCT:   ledDriver_init
 * BRIEF:   Initialize
 * RETURN:  Returns Nothing
 * ARG:     No Arguments
 ****************************************************************************************************/
void ledDriver_init(void)
{
    uint8_t i;
    Timer_A_initContinuousModeParam initContinuousModeParam;

    /*** Initialize ***/
    /* Initialize GPIO Pins */
    GPIO_setAsOutputPin(DRIVER_CONFIG_LED_GPIO_LED_1_PORT, DRIVER_CONFIG_LED_GPIO_LED_1_PIN);
    GPIO_setAsOutputPin(DRIVER_CONFIG_LED_GPIO_LED_2_PORT, DRIVER_CONFIG_LED_GPIO_LED_2_PIN);

    /* Timer */
    initContinuousModeParam.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    initContinuousModeParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    initContinuousModeParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    initContinuousModeParam.timerClear = TIMER_A_DO_CLEAR;
    initContinuousModeParam.startTimer = true;
    Timer_A_initContinuousMode(DRIVER_CONFIG_LED_TIMER, &initContinuousModeParam);

    /* Reset Blink And Set Initial LED States To Off */
    for(i = 0; i < LED_DRIVER_LED_COUNT; i++)
    {
    	ledDriver_resetLedBlink((ledDriver_led_t)i);
        ledDriver_setState((ledDriver_led_t)i, false);
    }
}

/****************************************************************************************************
 * FUNCT:   ledDriver_isEnabled
 * BRIEF:   Is Enabled
 * RETURN:  bool: Enabled (true) Or Not Enabled (false)
 * ARG:     Led: LED
 ****************************************************************************************************/
bool ledDriver_isEnabled(const ledDriver_led_t Led)
{
	bool enabled;

	/*** Is Enabled ***/
    switch(Led)
    {
        case LED_DRIVER_LED_1:
        case LED_DRIVER_LED_2:
        	enabled = ledDriver_ledBlink[Led].enabled;
            break;
        case LED_DRIVER_LED_COUNT:
        default:
            enabled = false;
            break;
    }

	return enabled;
}

/****************************************************************************************************
 * FUNCT:   ledDriver_setState
 * BRIEF:   Set State
 * RETURN:  Returns Nothing
 * ARG:     Led: LED
 * ARG:     On: On (true) Or Off (false)
 ****************************************************************************************************/
void ledDriver_setState(const ledDriver_led_t Led, const bool On)
{
    /*** Set State ***/
    switch(Led)
    {
        case LED_DRIVER_LED_1:
            if(On)
                GPIO_setOutputHighOnPin(DRIVER_CONFIG_LED_GPIO_LED_1_PORT, DRIVER_CONFIG_LED_GPIO_LED_1_PIN);
            else
                GPIO_setOutputLowOnPin(DRIVER_CONFIG_LED_GPIO_LED_1_PORT, DRIVER_CONFIG_LED_GPIO_LED_1_PIN);
            break;
        case LED_DRIVER_LED_2:
            if(On)
                GPIO_setOutputHighOnPin(DRIVER_CONFIG_LED_GPIO_LED_2_PORT, DRIVER_CONFIG_LED_GPIO_LED_2_PIN);
            else
                GPIO_setOutputLowOnPin(DRIVER_CONFIG_LED_GPIO_LED_2_PORT, DRIVER_CONFIG_LED_GPIO_LED_2_PIN);
            break;
        case LED_DRIVER_LED_COUNT:
        default:
            /* Do Nothing */
            break;
    }
}

/****************************************************************************************************
 * FUNCT:   ledDriver_timerInterruptHandler
 * BRIEF:   Timer Interrupt Handler
 * RETURN:  Returns Nothing
 *  ARG:     InterruptFlag: Interrupt Flag
 ****************************************************************************************************/
void ledDriver_timerInterruptHandler(uint16_t InterruptFlag)
{
    uint16_t captureCompareRegister = 0, compareValue;
    ledDriver_led_t led = LED_DRIVER_LED_COUNT;

    /*** Handle Interrupt ***/
    switch(InterruptFlag)
    {
        case TA0IV_NONE:
            /*** No Interrupt ***/
            break;
        case TA0IV_TACCR1:
            /*** Capture/Compare Register 1 Interrupt ***/
            captureCompareRegister = DRIVER_CONFIG_LED_TIMER_LED_1_CCR;
            led = LED_DRIVER_LED_1;
            break;
        case TA0IV_TACCR2:
            /*** Capture/Compare Register 2 Interrupt ***/
            captureCompareRegister = DRIVER_CONFIG_LED_TIMER_LED_2_CCR;
            led = LED_DRIVER_LED_2;
            break;
        case TA0IV_3:
        case TA0IV_4:
        case TA0IV_5:
        case TA0IV_6:
            /*** Reserved ***/
            break;
        case TA0IV_TAIFG:
            /*** Timer Overflow Interrupt ***/
            break;
    }

    /*** Toggle LED, If Necessary ***/
    if(led != LED_DRIVER_LED_COUNT)
    {
        /* Set New LED State */
        ledDriver_ledBlink[led].on = !ledDriver_ledBlink[led].on;
        ledDriver_setState(led, ledDriver_ledBlink[led].on);

        /* Set New Toggle Period */
        if(ledDriver_ledBlink[led].on)
            compareValue = Timer_A_getCaptureCompareCount(DRIVER_CONFIG_LED_TIMER, captureCompareRegister) + LED_DRIVER_CONVERT_MILLISECONDS_TO_TICKS(ledDriver_ledBlink[led].onMilliseconds);
        else
            compareValue = Timer_A_getCaptureCompareCount(DRIVER_CONFIG_LED_TIMER, captureCompareRegister) + LED_DRIVER_CONVERT_MILLISECONDS_TO_TICKS(ledDriver_ledBlink[led].offMilliseconds);
        Timer_A_setCompareValue(DRIVER_CONFIG_LED_TIMER, captureCompareRegister, compareValue);
    }
}

/****************************************************************************************************
 * Function Definitions (Private)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   ledDriver_resetLedBlink
 * BRIEF:   Reset LED Blink
 * RETURN:  Returns Nothing
 * ARG:     Led: LED
 ****************************************************************************************************/
static void ledDriver_resetLedBlink(const ledDriver_led_t Led)
{
	/*** Reset LED Blink ***/
    switch(Led)
    {
        case LED_DRIVER_LED_1:
        case LED_DRIVER_LED_2:
        	ledDriver_ledBlink[Led].enabled = false;
            ledDriver_ledBlink[Led].on = false;
            ledDriver_ledBlink[Led].onMilliseconds = 0;
            ledDriver_ledBlink[Led].offMilliseconds = 0;
            break;
        case LED_DRIVER_LED_COUNT:
        default:
            /* Do Nothing */
        	break;
    }
}
