/****************************************************************************************************
 * FILE:    system_test.c
 * BRIEF:   System Test Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "cs.h"
#include "driver_config.h"
#include "gpio.h"
#include "hw_memmap.h"
#include "intrinsics.h"
#include "msp430fr6989.h"
#include <stdint.h>
#include "system.h"
#include "unity_fixture.h"

/****************************************************************************************************
 * Test Group
 ****************************************************************************************************/

TEST_GROUP(system_test);

/****************************************************************************************************
 * Test Setup And Teardown
 ****************************************************************************************************/

TEST_SETUP(system_test)
{
    /*** Set Up ***/
    /* Simulation */
    hwMemmap_init();
    
    /* Application */
    system_init();
}

TEST_TEAR_DOWN(system_test)
{
}

/****************************************************************************************************
 * Tests
 ****************************************************************************************************/

TEST(system_test, init)
{
    /********************************************************************************
     * Test 1: Initialize
     ********************************************************************************/
    
    /*** Subtest 1: Watchdog Disabled/Stopped ***/
    TEST_ASSERT_EQUAL_HEX16(WDTHOLD, HWREG16(WDT_A_BASE + OFS_WDTCTL) & WDTHOLD); // Disabled/Stopped: WDTCTL.WDTHOLD = 1b
    
    /*** Subtest 2: GPIO Power-On Default High-Impedance Mode Disabled ***/
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(PMM_BASE + OFS_PM5CTL0) & LOCKLPM5); // Disabled/Unlocked: PM5CTL0.LOCKLPM5 = 0b
    
    /*** Subtest 3: Interrupt Enabled ***/
    TEST_ASSERT_TRUE(intrinsics_getInterruptsEnabled());
}

TEST(system_test, initClocks)
{
    uint16_t baseAddress;
    
    /********************************************************************************
     * Test 1: Frequencies
     ********************************************************************************/
    
    /*** Subtest 1: Low-Frequency Crystal Oscillator (LFXT) ***/
    TEST_ASSERT_EQUAL_UINT32(32768, privateLFXTClockFrequency);
    
    /*** Subtest 2: High-Frequency Crystal Oscillator (HFXT) ***/
    TEST_ASSERT_EQUAL_UINT32(0, privateHFXTClockFrequency);
    
    /********************************************************************************
     * Test 2: Low-Frequency Crystal Oscillator (LFXT)
     ********************************************************************************/
    
    /*** Subtest 1: GPIO Pins ***/
    /* In */
    baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_LFXT_GPIO_IN_PORT];
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PADIR) & DRIVER_CONFIG_LFXT_GPIO_IN_PIN); // Input: PxDIR = 0b
    TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_LFXT_GPIO_IN_PIN, HWREG16(baseAddress + OFS_PASEL0) & DRIVER_CONFIG_LFXT_GPIO_IN_PIN); // Primary Module Function: PxSEL0 = 1b
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PASEL1) & DRIVER_CONFIG_LFXT_GPIO_IN_PIN); // Primary Module Function: PxSEL1 = 0b
    
    /* Out */
    baseAddress = GPIO_PORT_TO_BASE[DRIVER_CONFIG_LFXT_GPIO_OUT_PORT];
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PADIR) & DRIVER_CONFIG_LFXT_GPIO_OUT_PIN); // Input: PxDIR = 0b
    TEST_ASSERT_EQUAL_HEX16(DRIVER_CONFIG_LFXT_GPIO_OUT_PIN, HWREG16(baseAddress + OFS_PASEL0) & DRIVER_CONFIG_LFXT_GPIO_OUT_PIN); // Primary Module Function: PxSEL0 = 1b
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(baseAddress + OFS_PASEL1) & DRIVER_CONFIG_LFXT_GPIO_OUT_PIN); // Primary Module Function: PxSEL1 = 0b
    
    /*** Subtest 2: Turned Enabled And On ***/
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(CS_BASE + OFS_CSCTL4) & LFXTOFF); // On: CTL4.LFXTOFF = 0b
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(CS_BASE + OFS_CSCTL4) & LFXTBYPASS); // Source Is External Crystal: CSCTL4.LFXTBYPASS = 0b
    TEST_ASSERT_EQUAL_HEX16(CS_LFXT_DRIVE_3, HWREG16(CS_BASE + OFS_CSCTL4) & CS_LFXT_DRIVE_3); // Maximum Drive Strength: CTL4.LFXTDRIVE = 11b
    
    /********************************************************************************
     * Test 3: System Clocks
     ********************************************************************************/
    
    /*** Subtest 1: Digitally Controlled Oscillator (DCO) ***/
    TEST_ASSERT_EQUAL_HEX16(0, HWREG16(CS_BASE + OFS_CSCTL1) & DCORSEL); // Low Speeed Device: CTL1.DCORSEL = 0b
    TEST_ASSERT_EQUAL_HEX16(CS_DCOFSEL_6, HWREG16(CS_BASE + OFS_CSCTL1) & DCOFSEL); // 8 MHz: CTL1.DCORSEL = 011b
    
    /*** Subtest 2: Auxiliary Clock (ACLK) ***/
    TEST_ASSERT_EQUAL_HEX16(CS_LFXTCLK_SELECT << 8, HWREG16(CS_BASE + OFS_CSCTL2) & SELA_7); // LFXT Is Source: CTL2.SELA = 000b
    TEST_ASSERT_EQUAL_HEX16(CS_CLOCK_DIVIDER_1 << 8, HWREG16(CS_BASE + OFS_CSCTL3) & DIVA); // Clock Divider Is 1: 000b
    
    /*** Subtest 3: Master Clock (MCLK) ***/
    TEST_ASSERT_EQUAL_HEX16(CS_DCOCLK_SELECT, HWREG16(CS_BASE + OFS_CSCTL2) & SELM_7); // DCO Is Source: CTL2.SELM = 011b
    TEST_ASSERT_EQUAL_HEX16(CS_CLOCK_DIVIDER_1, HWREG16(CS_BASE + OFS_CSCTL3) & DIVM); // Clock Divider Is 1: 000b
    
    /*** Subtest 4: Sub-Main CLock (SMCLK) ***/
    TEST_ASSERT_EQUAL_HEX16(CS_DCOCLK_SELECT << 4, HWREG16(CS_BASE + OFS_CSCTL2) & SELS_7); // DCO Is Source: CTL2.SELS = 011b
    TEST_ASSERT_EQUAL_HEX16(CS_CLOCK_DIVIDER_1 << 4, HWREG16(CS_BASE + OFS_CSCTL3) & DIVS); // Clock Divider Is 1: 000b
}

TEST(system_test, reset)
{
    /********************************************************************************
     * Test 1: Reset
     ********************************************************************************/
    
    /* Subtest 1: System Reset */
    system_reset();
    TEST_ASSERT_EQUAL_HEX16(~WDTPW, HWREG16(WDT_A_BASE + OFS_WDTCTL)); // If WDTPW Isn't Written A Power Up Clear (PUC) Is Generated
}

/****************************************************************************************************
 * Test Group Runner
 ****************************************************************************************************/

TEST_GROUP_RUNNER(system_test)
{
    RUN_TEST_CASE(system_test, init)
    RUN_TEST_CASE(system_test, initClocks)
    RUN_TEST_CASE(system_test, reset)
}
