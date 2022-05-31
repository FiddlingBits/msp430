/****************************************************************************************************
 * FILE:    lcd_driver_test.c
 * BRIEF:   LCD (Liquid Crystal Display) Driver Test Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "hw_memmap.h"
#include "lcd_c.h"
#include "lcd_driver.h"
#include "msp430fr6989.h"
#include <stdbool.h>
#include "system.h"
#include "unity_fixture.h"

/****************************************************************************************************
 * Test Group
 ****************************************************************************************************/

TEST_GROUP(lcd_driver_test);

/****************************************************************************************************
 * Test Setup And Teardown
 ****************************************************************************************************/

TEST_SETUP(lcd_driver_test)
{
    /*** Set Up ***/
    /* Simulation */
    hwMemmap_init();
    
    /* Application */
    system_init();
}

TEST_TEAR_DOWN(lcd_driver_test)
{
}

/****************************************************************************************************
 * Tests
 ****************************************************************************************************/

TEST(lcd_driver_test, init)
{
    /********************************************************************************
     * Test 1: Initialize
     ********************************************************************************/

    /*** Subtest 1: GPIO Pins ***/
	TEST_ASSERT_EQUAL_HEX16(0xFFD0, HWREG16(LCD_C_BASE + OFS_LCDCPCTL0)); // 0xFFD0: Left To Right: Pins 15 To 6 And Pin 4
	TEST_ASSERT_EQUAL_HEX16(0xF83F, HWREG16(LCD_C_BASE + OFS_LCDCPCTL1)); // 0xF83F: Left To Right: Pins 31 To 27 And 21 To 16
	TEST_ASSERT_EQUAL_HEX16(0x00F8, HWREG16(LCD_C_BASE + OFS_LCDCPCTL2)); // 0x00F8: Left To Right: Pins 39 To 35

	/*** Subtest 2: LCD ***/
	TEST_ASSERT_EQUAL_HEX16(LCD_C_CLOCKSOURCE_ACLK, HWREG16(LCD_C_BASE + OFS_LCDCCTL0) & LCDSSEL); // Auxiliary Clock: LCDCCTL0.LCDSSEL = 0b
	TEST_ASSERT_EQUAL_HEX16(LCD_C_CLOCKDIVIDER_1, HWREG16(LCD_C_BASE + OFS_LCDCCTL0) & LCDDIV); // Clock Divider 1: LCDCCTL0.LCDDIVx = 00000b
	TEST_ASSERT_EQUAL_HEX16(LCD_C_CLOCKPRESCALAR_16, HWREG16(LCD_C_BASE + OFS_LCDCCTL0) & LCDPRE); // Clock Prescaler 16: LCDCCTL0.LCDPREx = 100b
	TEST_ASSERT_EQUAL_HEX16(LCD_C_4_MUX, HWREG16(LCD_C_BASE + OFS_LCDCCTL0) & LCDMX); // MUX Rate 4: LCDCCTL0.LCDMXx = 011b
	TEST_ASSERT_EQUAL_HEX16(LCD_C_LOW_POWER_WAVEFORMS, HWREG16(LCD_C_BASE + OFS_LCDCCTL0) & LCDLP); // Low Power Wavforms: LCDCCTL0.LCDLP = 1b
	TEST_ASSERT_EQUAL_HEX16(LCD_C_SEGMENTS_ENABLED, HWREG16(LCD_C_BASE + OFS_LCDCCTL0) & LCDSON); // Segments Enabled: LCDCCTL0.LCDSON = 1b

	/*** Subtest 3: Voltage ***/
	TEST_ASSERT_EQUAL_HEX16(LCD_C_VLCD_GENERATED_INTERNALLY, HWREG16(LCD_C_BASE + OFS_LCDCVCTL) & VLCDEXT); // VLCD Generated Internally: LCDCVCTL.VLCDEXT = 0b
	TEST_ASSERT_EQUAL_HEX16(LCD_C_V2V3V4_GENERATED_INTERNALLY_NOT_SWITCHED_TO_PINS, HWREG16(LCD_C_BASE + OFS_LCDCVCTL) & (LCDREXT | LCDEXTBIAS)); // V2 To V4 Generated Interally: LCDCVCTL.LCDEXTBIAS = 0b; Internally Genreated V2 To V4 Not Switched To Pins: LCDCVCTL.LCDREXT = 0b
	TEST_ASSERT_EQUAL_HEX16(LCD_C_V5_VSS, HWREG16(LCD_C_BASE + OFS_LCDCVCTL) & R03EXT); // V5 Is VSS: LCDCVCTL.R03EXT = 0b
	TEST_ASSERT_EQUAL_HEX16(LCD_C_CHARGEPUMP_VOLTAGE_2_60V_OR_2_17VREF, HWREG16(LCD_C_BASE + OFS_LCDCVCTL) & VLCD); // Charge Pump Voltage Select: 2.60 V: LCDCVCTL.VLCDx = 00001b

	/*** Subtest 4: Charge Pump ***/
	TEST_ASSERT_EQUAL_HEX16(LCDCPEN, HWREG16(LCD_C_BASE + OFS_LCDCVCTL) & LCDCPEN); // Charge Pump Enabled: LCDCVCTL.LCDCPEN = 1b
	TEST_ASSERT_EQUAL_HEX16(LCD_C_INTERNAL_REFERENCE_VOLTAGE, HWREG16(LCD_C_BASE + OFS_LCDCVCTL) & VLCDREF); // Internal Charge Pump Reference: LCDCVCTL.VLCDREFx = 00b
	TEST_ASSERT_EQUAL_HEX16(LCD_C_SYNCHRONIZATION_ENABLED, HWREG16(LCD_C_BASE + OFS_LCDCCPCTL) & LCDCPCLKSYNC); // Synchronization Enabled: LCDCCPCTL.LCDCPCLKSYNC = 1b

	/*** Subtest 5: Blinking ***/
	TEST_ASSERT_EQUAL_HEX16(LCD_C_BLINK_FREQ_CLOCK_DIVIDER_1, HWREG16(LCD_C_BASE + OFS_LCDCBLKCTL) & LCDBLKDIV); // Clock Divider 1: LCDCBLKCTL.LCDBLKDIVx = 000b
	TEST_ASSERT_EQUAL_HEX16(LCD_C_BLINK_FREQ_CLOCK_PRESCALAR_32768, HWREG16(LCD_C_BASE + OFS_LCDCBLKCTL) & LCDBLKPRE); // Clock Prescaler 32768: LCDCBLKCTL.LCDBLKPREx = 110b
	TEST_ASSERT_EQUAL_HEX16(LCD_C_BLINK_MODE_INDIVIDUAL_SEGMENTS, HWREG16(LCD_C_BASE + OFS_LCDCBLKCTL) & LCDBLKMOD); // Individual Segment Blinking Mode: LCDCBLKCTL.LCDBLKMODx = 01b
}

TEST(lcd_driver_test, set)
{
    /********************************************************************************
     * Test 1: Set With Clear, Without Blink
     ********************************************************************************/

	/* Set Up */
	lcdDriver_set(LCD_C_SEGMENT_LINE_6, 0x0A, true, false); // true (Clear All Memory); false (Don't Blink)
	lcdDriver_set(LCD_C_SEGMENT_LINE_7, 0x0A, false, false); // false (Don't Clear All Memory); false (Don't Blink)

	/* Verify State As Expected */
	TEST_ASSERT_EQUAL_HEX8(0x0A, HWREG8(LCD_C_BASE + OFS_LCDM1 + (LCD_C_SEGMENT_LINE_6 / 2)) & 0x0F);
	TEST_ASSERT_EQUAL_HEX8(0, HWREG8(LCD_C_BASE + OFS_LCDBM1 + (LCD_C_SEGMENT_LINE_6 / 2)) & 0x0F);
	TEST_ASSERT_EQUAL_HEX8(0xA0, HWREG8(LCD_C_BASE + OFS_LCDM1 + (LCD_C_SEGMENT_LINE_7 / 2)) & 0xF0);
	TEST_ASSERT_EQUAL_HEX8(0, HWREG8(LCD_C_BASE + OFS_LCDBM1 + (LCD_C_SEGMENT_LINE_7 / 2)) & 0xF0);
	TEST_ASSERT_EQUAL_HEX16(LCDON, HWREG16(LCD_C_BASE + OFS_LCDCCTL0) & LCDON); // LCD On

	/* Set With Clear */
	lcdDriver_set(LCD_C_SEGMENT_LINE_6, 0x05, true, false); // true (Clear All Memory); false (Don't Blink)

	/* Verify State As Expected */
	TEST_ASSERT_EQUAL_HEX8(0x05, HWREG8(LCD_C_BASE + OFS_LCDM1 + (LCD_C_SEGMENT_LINE_6 / 2)) & 0x0F);
	TEST_ASSERT_EQUAL_HEX8(0, HWREG8(LCD_C_BASE + OFS_LCDBM1 + (LCD_C_SEGMENT_LINE_6 / 2)) & 0x0F);
	TEST_ASSERT_EQUAL_HEX8(0, HWREG8(LCD_C_BASE + OFS_LCDM1 + (LCD_C_SEGMENT_LINE_7 / 2)) & 0xF0);
	TEST_ASSERT_EQUAL_HEX8(0, HWREG8(LCD_C_BASE + OFS_LCDBM1 + (LCD_C_SEGMENT_LINE_7 / 2)) & 0xF0);
	TEST_ASSERT_EQUAL_HEX16(LCDON, HWREG16(LCD_C_BASE + OFS_LCDCCTL0) & LCDON); // LCD On

    /********************************************************************************
     * Test 2: Set Without Clear, Without Blink
     ********************************************************************************/

	/* Set Up */
	lcdDriver_set(LCD_C_SEGMENT_LINE_6, 0x0A, true, false); // true (Clear All Memory); false (Don't Blink)
	lcdDriver_set(LCD_C_SEGMENT_LINE_7, 0x0A, false, false); // false (Don't Clear All Memory); false (Don't Blink)

	/* Verify State As Expected */
	TEST_ASSERT_EQUAL_HEX8(0x0A, HWREG8(LCD_C_BASE + OFS_LCDM1 + (LCD_C_SEGMENT_LINE_6 / 2)) & 0x0F);
	TEST_ASSERT_EQUAL_HEX8(0, HWREG8(LCD_C_BASE + OFS_LCDBM1 + (LCD_C_SEGMENT_LINE_6 / 2)) & 0x0F);
	TEST_ASSERT_EQUAL_HEX8(0xA0, HWREG8(LCD_C_BASE + OFS_LCDM1 + (LCD_C_SEGMENT_LINE_7 / 2)) & 0xF0);
	TEST_ASSERT_EQUAL_HEX8(0, HWREG8(LCD_C_BASE + OFS_LCDBM1 + (LCD_C_SEGMENT_LINE_7 / 2)) & 0xF0);
	TEST_ASSERT_EQUAL_HEX16(LCDON, HWREG16(LCD_C_BASE + OFS_LCDCCTL0) & LCDON); // LCD On

	/* Set With Clear */
	lcdDriver_set(LCD_C_SEGMENT_LINE_6, 0x05, false, false); // false (Don't Clear All Memory); false (Don't Blink)

	/* Verify State As Expected */
	TEST_ASSERT_EQUAL_HEX8(0x05, HWREG8(LCD_C_BASE + OFS_LCDM1 + (LCD_C_SEGMENT_LINE_6 / 2)) & 0x0F);
	TEST_ASSERT_EQUAL_HEX8(0, HWREG8(LCD_C_BASE + OFS_LCDBM1 + (LCD_C_SEGMENT_LINE_6 / 2)) & 0x0F);
	TEST_ASSERT_EQUAL_HEX8(0xA0, HWREG8(LCD_C_BASE + OFS_LCDM1 + (LCD_C_SEGMENT_LINE_7 / 2)) & 0xF0);
	TEST_ASSERT_EQUAL_HEX8(0, HWREG8(LCD_C_BASE + OFS_LCDBM1 + (LCD_C_SEGMENT_LINE_7 / 2)) & 0xF0);
	TEST_ASSERT_EQUAL_HEX16(LCDON, HWREG16(LCD_C_BASE + OFS_LCDCCTL0) & LCDON); // LCD On

    /********************************************************************************
     * Test 3: Set With Clear, With Blink
     ********************************************************************************/

	/* Set With Blink */
	lcdDriver_set(LCD_C_SEGMENT_LINE_6, 0x0A, true, true); // true (Clear All Memory); true (blink)

	/* Verify State As Expected */
	TEST_ASSERT_EQUAL_HEX8(0x0A, HWREG8(LCD_C_BASE + OFS_LCDM1 + (LCD_C_SEGMENT_LINE_6 / 2)) & 0x0F);
	TEST_ASSERT_EQUAL_HEX8(0x0A, HWREG8(LCD_C_BASE + OFS_LCDBM1 + (LCD_C_SEGMENT_LINE_6 / 2)) & 0x0F);
	TEST_ASSERT_EQUAL_HEX16(LCDON, HWREG16(LCD_C_BASE + OFS_LCDCCTL0) & LCDON); // LCD On
}

/****************************************************************************************************
 * Test Group Runner
 ****************************************************************************************************/

TEST_GROUP_RUNNER(lcd_driver_test)
{
    RUN_TEST_CASE(lcd_driver_test, init)
	RUN_TEST_CASE(lcd_driver_test, set)
}
