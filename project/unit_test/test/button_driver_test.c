/****************************************************************************************************
 * FILE:    button_driver_test.c
 * BRIEF:   Button Driver Test Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "button_driver.h"
#include "button_driver_test.h"
#include "driver_config.h"
#include "gpio.h"
#include "hw_memmap.h"
#include "led_driver_test.h"
#include <stdint.h>
#include "system.h"
#include "unity_fixture.h"

/****************************************************************************************************
 * Test Group
 ****************************************************************************************************/

TEST_GROUP(button_driver_test);

/****************************************************************************************************
 * Test Setup And Teardown
 ****************************************************************************************************/

TEST_SETUP(button_driver_test)
{
    /*** Set Up ***/
    /* Simulation */
    hwMemmap_init();
    
    /* Application */
    system_init();
}

TEST_TEAR_DOWN(button_driver_test)
{
}

/****************************************************************************************************
 * Tests
 ****************************************************************************************************/

TEST(button_driver_test, init)
{
	uint16_t baseAddress;

    /********************************************************************************
     * Test 1: Initialize
     ********************************************************************************/

    /*** Subtest 1: GPIO Pins ***/
    /* Button 1 */
    baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PORT];
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PADIR) & DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN); // Input: PxDIR = 0b
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PASEL0) & DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN); // General Purpose I/O: PxSEL0 = 0b
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PASEL1) & DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN); // General Purpose I/O: PxSEL1 = 0b
    TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN, HWREG16(baseAddress + OFS_PAREN) & DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN); // Pull-Up/Pull-Down Resistor Enabled: PAREN = 1b
    TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN, HWREG16(baseAddress + OFS_PAOUT) & DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN); // Pull-Up Resistor Selected: PAOUT = 1b

    /* Button 2 */
    baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PORT];
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PADIR) & DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PIN); // Input: PxDIR = 0b
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PASEL0) & DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PIN); // General Purpose I/O: PxSEL0 = 0b
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PASEL1) & DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PIN); // General Purpose I/O: PxSEL1 = 0b
    TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PIN, HWREG16(baseAddress + OFS_PAREN) & DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PIN); // Pull-Up/Pull-Down Resistor Enabled: PAREN = 1b
    TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PIN, HWREG16(baseAddress + OFS_PAOUT) & DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PIN); // Pull-Up Resistor Selected: PAOUT = 1b

    /*** Subtest 2: Interrupts ***/
    /* Button 1 */
    baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PORT];
    TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN, HWREG16(baseAddress + OFS_PAIES) & DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN); // High To Low Edge Transition: PAIES = 1b
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PAIFG) & DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN); // Interrupt Cleared: PAIFG = 0b
    TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN, HWREG16(baseAddress + OFS_PAIE) & DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN); // Interrupt Enabled: PAIE = 1b

    /* Button 2 */
    baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PORT];
    TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PIN, HWREG16(baseAddress + OFS_PAIES) & DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PIN); // High To Low Edge Transition: PAIES = 1b
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PAIFG) & DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PIN); // Interrupt Cleared: PAIFG = 0b
    TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PIN, HWREG16(baseAddress + OFS_PAIE) & DRIVER_CONFIG_BUTTON_GPIO_BUTTON_2_PIN); // Interrupt Enabled: PAIE = 1b

    /*** Subtest 3: Variables ***/
    TEST_ASSERT_EQUAL_HEX8(0x00, buttonDriver_state);
}

TEST(button_driver_test, portInterruptHandler)
{
	uint8_t i;
	uint16_t baseAddress;

    /********************************************************************************
     * Test 1: Interrupt
     ********************************************************************************/

	/* Set Up */
	baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PORT];
	buttonDriver_state = 0x00;

	/* Interrupt */
	for(i = 0; i < 10; i++)
	{
		/* Check State */
		if((i % 2) == 0)
		{
			TEST_ASSERT_EQUAL_HEX8(0x00, buttonDriver_state);
			TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN, HWREG16(baseAddress + OFS_PAIES) & DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN); // High To Low Edge Transition: PAIES = 1b

		}
		else
		{
			TEST_ASSERT_EQUAL_HEX8(BUTTON_DRIVER_BUTTON_1, buttonDriver_state);
			TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PAIES) & DRIVER_CONFIG_BUTTON_GPIO_BUTTON_1_PIN); // Low To High Edge Transition: PAIES = 0b
		}

		/* Interrupt */
		buttonDriver_portInterruptHandler(P1IV_P1IFG1);
	}
}

TEST(button_driver_test, tick)
{
	uint8_t i;
	uint16_t baseAddress;

    /********************************************************************************
     * Test 1: LED Blink Not Enabled
     ********************************************************************************/

	/* Set Up */
	baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_LED_GPIO_LED_1_PORT];
	ledDriver_setState(LED_DRIVER_LED_1, false);

	/* Tick */
	for(i = 0; i < 10; i++)
	{
	    /* LED State */
		if((i % 2) == 0)
			TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PAOUT) & DRIVER_CONFIG_LED_GPIO_LED_1_PIN); // Off
		else
			TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_LED_GPIO_LED_1_PIN, HWREG16(baseAddress + OFS_PAOUT) & DRIVER_CONFIG_LED_GPIO_LED_1_PIN); // On

		/* Toggle Button State */
		buttonDriver_state ^= BUTTON_DRIVER_BUTTON_1;

		/* Tick */
		buttonDriver_tick();
	}

    /********************************************************************************
     * Test 2: LED Blink Enabled
     ********************************************************************************/

	/* Set Up */
	baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_LED_GPIO_LED_2_PORT];
	ledDriver_ledBlink[LED_DRIVER_LED_2].enabled = true;
	ledDriver_setState(LED_DRIVER_LED_2, false);

	/* Tick */
	for(i = 0; i < 10; i++)
	{
	    /* LED Off */
	    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PAOUT) & DRIVER_CONFIG_LED_GPIO_LED_2_PIN);

		/* Toggle Button State */
		buttonDriver_state ^= BUTTON_DRIVER_BUTTON_2;

		/* Tick */
		buttonDriver_tick();
	}

	/* Clean Up */
	ledDriver_ledBlink[LED_DRIVER_LED_2].enabled = false;
}

/****************************************************************************************************
 * Test Group Runner
 ****************************************************************************************************/

TEST_GROUP_RUNNER(button_driver_test)
{
    RUN_TEST_CASE(button_driver_test, init)
	RUN_TEST_CASE(button_driver_test, portInterruptHandler)
	RUN_TEST_CASE(button_driver_test, tick)
}
