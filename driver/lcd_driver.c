/****************************************************************************************************
 * FILE:    lcd_driver.c
 * BRIEF:   LCD (Liquid Crystal Display) Driver Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "lcd_c.h"
#include "lcd_driver.h"
#include "msp430fr6989.h"

/****************************************************************************************************
 * Function Definitions (Public)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   lcdDriver_init
 * BRIEF:   Initialize
 * RETURN:  Returns Nothing
 * ARG:     No Arguments
 ****************************************************************************************************/
void lcdDriver_init(void)
{
	LCD_C_initParam lcdInit;

	/*** Initialize ***/
	/* GPIO Pins */
	LCD_C_setPinAsLCDFunctionEx(LCD_C_BASE, LCD_C_SEGMENT_LINE_4, LCD_C_SEGMENT_LINE_4);
	LCD_C_setPinAsLCDFunctionEx(LCD_C_BASE, LCD_C_SEGMENT_LINE_6, LCD_C_SEGMENT_LINE_21);
	LCD_C_setPinAsLCDFunctionEx(LCD_C_BASE, LCD_C_SEGMENT_LINE_27, LCD_C_SEGMENT_LINE_31);
	LCD_C_setPinAsLCDFunctionEx(LCD_C_BASE, LCD_C_SEGMENT_LINE_35, LCD_C_SEGMENT_LINE_39);

	/* LCD */
	lcdInit.clockSource = LCD_C_CLOCKSOURCE_ACLK;
	lcdInit.clockDivider = LCD_C_CLOCKDIVIDER_1;
	lcdInit.clockPrescalar = LCD_C_CLOCKPRESCALAR_16;
	lcdInit.muxRate = LCD_C_4_MUX;
	lcdInit.waveforms = LCD_C_LOW_POWER_WAVEFORMS;
	lcdInit.segments = LCD_C_SEGMENTS_ENABLED;
	LCD_C_init(LCD_C_BASE, &lcdInit);

	/* Voltage */
	LCD_C_setVLCDSource(LCD_C_BASE, LCD_C_VLCD_GENERATED_INTERNALLY, LCD_C_V2V3V4_GENERATED_INTERNALLY_NOT_SWITCHED_TO_PINS, LCD_C_V5_VSS);
	LCD_C_setVLCDVoltage(LCD_C_BASE, LCD_C_CHARGEPUMP_VOLTAGE_2_60V_OR_2_17VREF);

	/* Charge Pump */
	LCD_C_enableChargePump(LCD_C_BASE);
	LCD_C_selectChargePumpReference(LCD_C_BASE, LCD_C_INTERNAL_REFERENCE_VOLTAGE);
	LCD_C_configChargePump(LCD_C_BASE, LCD_C_SYNCHRONIZATION_ENABLED, 0);

	/* Blinking */
	LCD_C_setBlinkingControl(LCD_C_BASE, LCD_C_BLINK_FREQ_CLOCK_DIVIDER_1, LCD_C_BLINK_FREQ_CLOCK_PRESCALAR_32768, LCD_C_BLINK_MODE_INDIVIDUAL_SEGMENTS); // Blinking Frequency = 32768 Hz (ACLK Frequency) / (1 * 32768) = 1 Hz
}

/****************************************************************************************************
 * FUNCT:   lcdDriver_set
 * BRIEF:   Set
 * RETURN:  Returns Nothing
 * ARG:     Segment: Segment
 * ARG:		Value: Value
 * ARG:		Clear: Clear All Memory (true) Or Don't Clear All Memory (false)
 * ARG:		Blink: Blink (true) Or Don't Blink (false) Segment
 ****************************************************************************************************/
void lcdDriver_set(const uint8_t Segment, const uint8_t Value, const bool Clear, const bool Blink)
{
	/*** Set ***/
	LCD_C_off(LCD_C_BASE);
	if(Clear)
	{
		LCD_C_clearMemory(LCD_C_BASE);
		LCD_C_clearBlinkingMemory(LCD_C_BASE);
	}
	LCD_C_setMemory(LCD_C_BASE, Segment, Value);
	if(Blink)
	    LCD_C_setBlinkingMemory(LCD_C_BASE, Segment, Value);
	LCD_C_on(LCD_C_BASE);
}
