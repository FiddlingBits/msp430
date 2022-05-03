/****************************************************************************************************
 * FILE:    led_driver_test.c
 * BRIEF:   LED (Light-Emitting Diode) Driver Test Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "driver_config.h"
#include "gpio.h"
#include "hw_memmap.h"
#include "led_driver.h"
#include "led_driver_test.h"
#include <stdbool.h>
#include "system.h"
#include "unity_fixture.h"

/****************************************************************************************************
 * Test Group
 ****************************************************************************************************/

TEST_GROUP(led_driver_test);

/****************************************************************************************************
 * Test Setup And Teardown
 ****************************************************************************************************/

TEST_SETUP(led_driver_test)
{
    /*** Set Up ***/
    /* Simulation */
    hwMemmap_init();
    
    /* Application */
    system_init();
}

TEST_TEAR_DOWN(led_driver_test)
{
}

/****************************************************************************************************
 * Tests
 ****************************************************************************************************/

TEST(led_driver_test, enableBlink)
{
    uint16_t baseAddress, expectedCompareValue, offMilliseconds, onMilliseconds;
    
    /********************************************************************************
     * Test 1: Errors
     ********************************************************************************/
    
    /*** Subtest 1: LED Out Of Range ***/
    TEST_ASSERT_FALSE(ledDriver_enableBlink(LED_DRIVER_LED_COUNT, 500, 250));
    
    /*** Subtest 2: On Milliseconds Out Of Range ***/
    TEST_ASSERT_FALSE(ledDriver_enableBlink(LED_DRIVER_LED_2, 2001, 250));
    
    /*** Subtest 3: Off Milliseconds Out Of Range ***/
    TEST_ASSERT_FALSE(ledDriver_enableBlink(LED_DRIVER_LED_2, 500, 2001));
    
    /********************************************************************************
     * Test 2: Solid On
     ********************************************************************************/
    
    /* Enable Blink: Solid On */
    TEST_ASSERT_TRUE(ledDriver_enableBlink(LED_DRIVER_LED_1, 1, 0)); // On Milliseconds Doesn't Matter As Long As It's Greater Than 0

    /* LED Structure Set */
    TEST_ASSERT_TRUE(ledDriver_ledBlink[LED_DRIVER_LED_1].enabled);
    TEST_ASSERT_FALSE(ledDriver_ledBlink[LED_DRIVER_LED_1].on); // Not Used
    TEST_ASSERT_EQUAL_UINT16(0, ledDriver_ledBlink[LED_DRIVER_LED_1].onMilliseconds); // Not Used
    TEST_ASSERT_EQUAL_UINT16(0, ledDriver_ledBlink[LED_DRIVER_LED_1].offMilliseconds); // Not Used
    
    /* LED On */
    baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_LED_GPIO_LED_1_PORT];
    TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_LED_GPIO_LED_1_PIN, HWREG16(baseAddress + OFS_PAOUT) & DRIVER_CONFIG_LED_GPIO_LED_1_PIN);
    
    /* Capture/Compare Interrupt Cleared And Disabled ***/
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(DRIVER_CONFIG_LED_TIMER + DRIVER_CONFIG_LED_TIMER_LED_1_CCR) & CCIFG);
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(DRIVER_CONFIG_LED_TIMER + DRIVER_CONFIG_LED_TIMER_LED_1_CCR) & CCIE);
    
    /********************************************************************************
     * Test 3: Solid Off
     ********************************************************************************/

    /* Enable Blink: Solid Off */
    TEST_ASSERT_TRUE(ledDriver_enableBlink(LED_DRIVER_LED_2, 0, 1)); // Off Milliseconds Doesn't Matter As Long As It's Greater Than 0

    /* LED Structure Set */
    TEST_ASSERT_TRUE(ledDriver_ledBlink[LED_DRIVER_LED_2].enabled);
    TEST_ASSERT_FALSE(ledDriver_ledBlink[LED_DRIVER_LED_1].on); // Not Used
    TEST_ASSERT_EQUAL_UINT16(0, ledDriver_ledBlink[LED_DRIVER_LED_1].onMilliseconds); // Not Used
    TEST_ASSERT_EQUAL_UINT16(0, ledDriver_ledBlink[LED_DRIVER_LED_1].offMilliseconds); // Not Used
    
    /* LED Off */
    baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_LED_GPIO_LED_2_PORT];
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PAOUT) & DRIVER_CONFIG_LED_GPIO_LED_2_PIN);
    
    /* Capture/Compare Interrupt Cleared And Disabled ***/
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(DRIVER_CONFIG_LED_TIMER + DRIVER_CONFIG_LED_TIMER_LED_2_CCR) & CCIFG);
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(DRIVER_CONFIG_LED_TIMER + DRIVER_CONFIG_LED_TIMER_LED_2_CCR) & CCIE);
    
    /********************************************************************************
     * Test 4: Blinking
     ********************************************************************************/
    
    /* Set Up */
    onMilliseconds = 456;
    offMilliseconds = 987;
    
    /* Enable Blink: Blinking */
    TEST_ASSERT_TRUE(ledDriver_enableBlink(LED_DRIVER_LED_1, onMilliseconds, offMilliseconds));
    
    /* LED Structure Set */
    TEST_ASSERT_TRUE(ledDriver_ledBlink[LED_DRIVER_LED_1].enabled);
    TEST_ASSERT_TRUE(ledDriver_ledBlink[LED_DRIVER_LED_1].on); // true = On
    TEST_ASSERT_EQUAL_UINT16(onMilliseconds, ledDriver_ledBlink[LED_DRIVER_LED_1].onMilliseconds);
    TEST_ASSERT_EQUAL_UINT16(offMilliseconds, ledDriver_ledBlink[LED_DRIVER_LED_1].offMilliseconds);
    
    /* LED On */
    baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_LED_GPIO_LED_1_PORT];
    TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_LED_GPIO_LED_1_PIN, HWREG16(baseAddress + OFS_PAOUT) & DRIVER_CONFIG_LED_GPIO_LED_1_PIN);

    /* Capture/Compare Initialized */
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(DRIVER_CONFIG_LED_TIMER + DRIVER_CONFIG_LED_TIMER_LED_1_CCR) & CAP); // Compare Mode: TAxCCTLn.CAP = 0b
    TEST_ASSERT_EQUAL_HEX16(TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE, HWREG16(DRIVER_CONFIG_LED_TIMER + DRIVER_CONFIG_LED_TIMER_LED_1_CCR) & CCIE); // Interrupt Enabled: TAxCCTLn.CCIE = 1b
    TEST_ASSERT_EQUAL_HEX16(TIMER_A_OUTPUTMODE_OUTBITVALUE, HWREG16(DRIVER_CONFIG_LED_TIMER + DRIVER_CONFIG_LED_TIMER_LED_1_CCR) & OUTMOD); // Out Bit Value: TAxCCTLn.OUTMOD = 000b
    expectedCompareValue = LED_DRIVER_CONVERT_MILLISECONDS_TO_TICKS(onMilliseconds);
    TEST_ASSERT_EQUAL_HEX16(expectedCompareValue, HWREG16(DRIVER_CONFIG_LED_TIMER + DRIVER_CONFIG_LED_TIMER_LED_1_CCR + OFS_TAxR)); // Compare Value
    
    /* Timer Restarted */
    TEST_ASSERT_EQUAL_HEX16(TIMER_A_CONTINUOUS_MODE, HWREG16(DRIVER_CONFIG_LED_TIMER + OFS_TAxCTL) & MC); // Continuous Mode: TAxCTL.MC = 10b

    /********************************************************************************
     * Test 5: Disable
     ********************************************************************************/

    /* Set Up */
    ledDriver_ledBlink[LED_DRIVER_LED_2].enabled = true;

    /* Enable Blink: Disable */
    TEST_ASSERT_TRUE(ledDriver_enableBlink(LED_DRIVER_LED_2, 0, 0));

    /* LED Structure Set */
    TEST_ASSERT_FALSE(ledDriver_ledBlink[LED_DRIVER_LED_2].enabled);
    TEST_ASSERT_FALSE(ledDriver_ledBlink[LED_DRIVER_LED_2].on);
    TEST_ASSERT_EQUAL_UINT16(0, ledDriver_ledBlink[LED_DRIVER_LED_2].onMilliseconds);
    TEST_ASSERT_EQUAL_UINT16(0, ledDriver_ledBlink[LED_DRIVER_LED_2].offMilliseconds);
}

TEST(led_driver_test, init)
{
    uint16_t baseAddress;
    ledDriver_led_t led;
    
    /********************************************************************************
     * Test 1: Initialize
     ********************************************************************************/
    
    /*** Subtest 1: GPIO Pins ***/
    /* LED 1 */
    baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_LED_GPIO_LED_1_PORT];
    TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_LED_GPIO_LED_1_PIN, HWREG16(baseAddress + OFS_PADIR) & DRIVER_CONFIG_LED_GPIO_LED_1_PIN); // Output: PxDIR = 1b
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PASEL0) & DRIVER_CONFIG_LED_GPIO_LED_1_PIN); // General Purpose I/O: PxSEL0 = 0b
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PASEL1) & DRIVER_CONFIG_LED_GPIO_LED_1_PIN); // General Purpose I/O: PxSEL1 = 0b
    
    /* LED 2 */
    baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_LED_GPIO_LED_2_PORT];
    TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_LED_GPIO_LED_2_PIN, HWREG16(baseAddress + OFS_PADIR) & DRIVER_CONFIG_LED_GPIO_LED_2_PIN); // Output: PxDIR = 1b
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PASEL0) & DRIVER_CONFIG_LED_GPIO_LED_2_PIN); // General Purpose I/O: PxSEL0 = 0b
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PASEL1) & DRIVER_CONFIG_LED_GPIO_LED_2_PIN); // General Purpose I/O: PxSEL1 = 0b
    
    /*** Subtest 2: Timer ***/
    /* Initialized And Started */
    TEST_ASSERT_EQUAL_HEX16(TIMER_A_CLOCKSOURCE_DIVIDER_1, HWREG16(DRIVER_CONFIG_LED_TIMER + OFS_TAxEX0) & TAIDEX);
    TEST_ASSERT_EQUAL_HEX16(TIMER_A_CLOCKSOURCE_ACLK, HWREG16(DRIVER_CONFIG_LED_TIMER + OFS_TAxCTL) & TASSEL); // Auxiliary Clock: TAxCTL.TASSEL = 01b
    TEST_ASSERT_EQUAL_HEX16(TIMER_A_TAIE_INTERRUPT_DISABLE, HWREG16(DRIVER_CONFIG_LED_TIMER + OFS_TAxCTL) & TAIE); // Disabled: TAxCTL.TAIE = 0b
    TEST_ASSERT_EQUAL_HEX16(TIMER_A_DO_CLEAR, HWREG16(DRIVER_CONFIG_LED_TIMER + OFS_TAxCTL) & TACLR); // Cleared: TAxCTL.TACLR = 1b 
    TEST_ASSERT_EQUAL_HEX16(TIMER_A_CONTINUOUS_MODE, HWREG16(DRIVER_CONFIG_LED_TIMER + OFS_TAxCTL) & MC); // Continuous Mode: TAxCTL.MC = 10b
    
    /*** Subtest 3: Variables ***/
    for(led = LED_DRIVER_LED_1; led < LED_DRIVER_LED_COUNT; led++)
    {
    	TEST_ASSERT_FALSE(ledDriver_ledBlink[led].enabled);
        TEST_ASSERT_FALSE(ledDriver_ledBlink[led].on);
        TEST_ASSERT_EQUAL_UINT16(0, ledDriver_ledBlink[led].onMilliseconds);
        TEST_ASSERT_EQUAL_UINT16(0, ledDriver_ledBlink[led].offMilliseconds);
    }
    
    /*** Subtest 4: LED States (Off) ***/
    /* LED 1 */
    baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_LED_GPIO_LED_1_PORT];
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PAOUT) & DRIVER_CONFIG_LED_GPIO_LED_1_PIN);
    
    /* LED 2 */
    baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_LED_GPIO_LED_2_PORT];
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PAOUT) & DRIVER_CONFIG_LED_GPIO_LED_2_PIN);
}

TEST(led_driver_test, isEnabled)
{
    /********************************************************************************
     * Test 1: Is Enabled
     ********************************************************************************/

	/*** Subtest 1: Errors ***/
	TEST_ASSERT_FALSE(ledDriver_isEnabled(LED_DRIVER_LED_COUNT));

	/*** Subtest 2: Disabled ***/
	/* Set Up */
	ledDriver_ledBlink[LED_DRIVER_LED_1].enabled = false;

	/* Disabled */
	TEST_ASSERT_FALSE(ledDriver_isEnabled(LED_DRIVER_LED_1));

	/*** Subtest 3: Enabled ***/
	/* Set Up */
	ledDriver_ledBlink[LED_DRIVER_LED_2].enabled = true;

	/* Enabled */
	TEST_ASSERT_TRUE(ledDriver_isEnabled(LED_DRIVER_LED_2));
}

TEST(led_driver_test, setState)
{
	uint16_t baseAddress;

    /********************************************************************************
     * Test 1: Set State
     ********************************************************************************/

	/*** Subtest 1: Off ***/
	/* Set Up */
	ledDriver_setState(LED_DRIVER_LED_1, false);

	/* Off */
	baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_LED_GPIO_LED_1_PORT];
	TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PAOUT) & DRIVER_CONFIG_LED_GPIO_LED_1_PIN);

	/*** Subtest 2: On ***/
	/* Set Up */
	ledDriver_setState(LED_DRIVER_LED_2, true);

	/* On */
	baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_LED_GPIO_LED_2_PORT];
	TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_LED_GPIO_LED_2_PIN, HWREG16(baseAddress + OFS_PAOUT) & DRIVER_CONFIG_LED_GPIO_LED_2_PIN);
}

TEST(led_driver_test, timerInterruptHandler)
{
	uint8_t i;
	uint16_t baseAddress, expectedCompareValue = 0, offMilliseconds, onMilliseconds;

    /********************************************************************************
     * Test 1: Interrupt
     ********************************************************************************/

    /* Set Up */
	baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_LED_GPIO_LED_1_PORT];
    onMilliseconds = 654;
    offMilliseconds = 789;

    /* Enable Blink: Blinking */
    TEST_ASSERT_TRUE(ledDriver_enableBlink(LED_DRIVER_LED_1, onMilliseconds, offMilliseconds));

    /* Blinking */
    for(i = 0; i < 10; i++)
    {
    	/* Check State */
    	if((i % 2) == 0)
    	{
			/* LED Structure Set */
			TEST_ASSERT_TRUE(ledDriver_ledBlink[LED_DRIVER_LED_1].enabled); // Didn't Change
			TEST_ASSERT_TRUE(ledDriver_ledBlink[LED_DRIVER_LED_1].on); // true = On
			TEST_ASSERT_EQUAL_UINT16(onMilliseconds, ledDriver_ledBlink[LED_DRIVER_LED_1].onMilliseconds); // Didn't Change
			TEST_ASSERT_EQUAL_UINT16(offMilliseconds, ledDriver_ledBlink[LED_DRIVER_LED_1].offMilliseconds); // Didn't Change

			/* LED On */
			TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_LED_GPIO_LED_1_PIN, HWREG16(baseAddress + OFS_PAOUT) & DRIVER_CONFIG_LED_GPIO_LED_1_PIN);

			/* Capture/Compare Initialized */
			TEST_ASSERT_EQUAL_HEX16(0, HWREG16(DRIVER_CONFIG_LED_TIMER + DRIVER_CONFIG_LED_TIMER_LED_1_CCR) & CAP); // Compare Mode: TAxCCTLn.CAP = 0b
			TEST_ASSERT_EQUAL_HEX16(TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE, HWREG16(DRIVER_CONFIG_LED_TIMER + DRIVER_CONFIG_LED_TIMER_LED_1_CCR) & CCIE); // Interrupt Enabled: TAxCCTLn.CCIE = 1b
			TEST_ASSERT_EQUAL_HEX16(TIMER_A_OUTPUTMODE_OUTBITVALUE, HWREG16(DRIVER_CONFIG_LED_TIMER + DRIVER_CONFIG_LED_TIMER_LED_1_CCR) & OUTMOD); // Out Bit Value: TAxCCTLn.OUTMOD = 000b
			expectedCompareValue += LED_DRIVER_CONVERT_MILLISECONDS_TO_TICKS(onMilliseconds);
			TEST_ASSERT_EQUAL_HEX16(expectedCompareValue, HWREG16(DRIVER_CONFIG_LED_TIMER + DRIVER_CONFIG_LED_TIMER_LED_1_CCR + OFS_TAxR)); // Compare Value

			/* Timer Running */
			TEST_ASSERT_EQUAL_HEX16(TIMER_A_CONTINUOUS_MODE, HWREG16(DRIVER_CONFIG_LED_TIMER + OFS_TAxCTL) & MC); // Continuous Mode: TAxCTL.MC = 10b
    	}
    	else
    	{
			/* LED Structure Set */
			TEST_ASSERT_TRUE(ledDriver_ledBlink[LED_DRIVER_LED_1].enabled); // Didn't Change
			TEST_ASSERT_FALSE(ledDriver_ledBlink[LED_DRIVER_LED_1].on); // false = Off
			TEST_ASSERT_EQUAL_UINT16(onMilliseconds, ledDriver_ledBlink[LED_DRIVER_LED_1].onMilliseconds); // Didn't Change
			TEST_ASSERT_EQUAL_UINT16(offMilliseconds, ledDriver_ledBlink[LED_DRIVER_LED_1].offMilliseconds); // Didn't Change

			/* LED Off */
			TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PAOUT) & DRIVER_CONFIG_LED_GPIO_LED_1_PIN);

			/* Capture/Compare Initialized */
			TEST_ASSERT_EQUAL_HEX16(0, HWREG16(DRIVER_CONFIG_LED_TIMER + DRIVER_CONFIG_LED_TIMER_LED_1_CCR) & CAP); // Compare Mode: TAxCCTLn.CAP = 0b
			TEST_ASSERT_EQUAL_HEX16(TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE, HWREG16(DRIVER_CONFIG_LED_TIMER + DRIVER_CONFIG_LED_TIMER_LED_1_CCR) & CCIE); // Interrupt Enabled: TAxCCTLn.CCIE = 1b
			TEST_ASSERT_EQUAL_HEX16(TIMER_A_OUTPUTMODE_OUTBITVALUE, HWREG16(DRIVER_CONFIG_LED_TIMER + DRIVER_CONFIG_LED_TIMER_LED_1_CCR) & OUTMOD); // Out Bit Value: TAxCCTLn.OUTMOD = 000b
			expectedCompareValue += LED_DRIVER_CONVERT_MILLISECONDS_TO_TICKS(offMilliseconds);
			TEST_ASSERT_EQUAL_HEX16(expectedCompareValue, HWREG16(DRIVER_CONFIG_LED_TIMER + DRIVER_CONFIG_LED_TIMER_LED_1_CCR + OFS_TAxR)); // Compare Value

			/* Timer Running */
			TEST_ASSERT_EQUAL_HEX16(TIMER_A_CONTINUOUS_MODE, HWREG16(DRIVER_CONFIG_LED_TIMER + OFS_TAxCTL) & MC); // Continuous Mode: TAxCTL.MC = 10b
    	}

    	/* Interrupt */
    	ledDriver_timerInterruptHandler(TA0IV_TACCR1);
    }
}

/****************************************************************************************************
 * Test Group Runner
 ****************************************************************************************************/

TEST_GROUP_RUNNER(led_driver_test)
{
    RUN_TEST_CASE(led_driver_test, enableBlink)
    RUN_TEST_CASE(led_driver_test, init)
	RUN_TEST_CASE(led_driver_test, isEnabled)
	RUN_TEST_CASE(led_driver_test, setState)
	RUN_TEST_CASE(led_driver_test, timerInterruptHandler)
}
