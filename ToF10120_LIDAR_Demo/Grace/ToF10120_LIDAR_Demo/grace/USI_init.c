/*
 *  This file is automatically generated and does not require a license
 *
 *  ==== WARNING: CHANGES TO THIS GENERATED FILE WILL BE OVERWRITTEN ====
 *
 *  To make changes to the generated code, use the space between existing
 *      "USER CODE START (section: <name>)"
 *  and
 *      "USER CODE END (section: <name>)"
 *  comments, where <name> is a single word identifying the section.
 *  Only these sections will be preserved.
 *
 *  Do not move these sections within this file or change the START and
 *  END comments in any way.
 *  ==== ALL OTHER CHANGES WILL BE OVERWRITTEN WHEN IT IS REGENERATED ====
 *
 *  This file was generated from
 *      D:/ti/grace/grace_2_20_02_32/packages/ti/mcu/msp430/csl/communication/USI_init.xdt
 */

#include <msp430.h>

/* USER CODE START (section: USI_init_c_prologue) */
/* User defined includes, defines, global variables and functions */
/* USER CODE END (section: USI_init_c_prologue) */

/*
 *  ======== USI_graceInit ========
 *  Initialize Universal Serial Interface
 */
void USI_graceInit(void)
{
    /* USER CODE START (section: USI_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: USI_graceInit_prologue) */

    /* Disable USI */
    USICTL0 |= USISWRST;

    /* 
     * USI Control Register 0
     * 
     * USIPE7 -- USI function enabled
     * USIPE6 -- USI function enabled
     * ~USIPE5 -- USI function disabled
     * ~USILSB -- MSB first
     * USIMST -- Master mode
     * ~USIGE -- Output latch enable depends on shift clock
     * ~USIOE -- Output disabled
     * USISWRST -- USI logic held in reset state
     * 
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    USICTL0 = USIPE7 | USIPE6 | USIMST | USISWRST;

    /* 
     * USI Control Register 1
     * 
     * ~USICKPH -- Data is changed on the first SCLK edge and captured on the following edge
     * USII2C -- I2C mode enabled
     * ~USISTTIE -- Interrupt on START condition disabled
     * USIIE -- Interrupt enabled
     * ~USIAL -- No arbitration lost condition
     * ~USISTP -- No STOP condition received
     * ~USISTTIFG -- No START condition received. No interrupt pending
     * USIIFG -- Interrupt pending
     * 
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    USICTL1 = USII2C | USIIE | USIIFG;

    /* 
     * USI Clock Control Register
     * 
     * USIDIV_4 -- Divide by 16
     * USISSEL_2 -- SMCLK
     * USICKPL -- Inactive state is high
     * ~USISWCLK -- Input clock is low
     * 
     * Note: ~USISWCLK indicates that USISWCLK has value zero
     */
    USICKCTL = USIDIV_4 | USISSEL_2 | USICKPL;

    /* 
     * USI Bit Counter Register
     * 
     * ~USISCLREL -- SCL line is held low if USIIFG is set
     * ~USI16B -- 8-bit shift register mode. Low byte register USISRL is used
     * USIIFGCC -- USIIFG is not cleared automatically
     * ~USICNT4 -- USI bit count
     * ~USICNT3 -- USI bit count
     * ~USICNT2 -- USI bit count
     * ~USICNT1 -- USI bit count
     * ~USICNT0 -- USI bit count
     * 
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    USICNT = USIIFGCC;

    /* Enable USI */
    USICTL0 &= ~USISWRST;

    /* USER CODE START (section: USI_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: USI_graceInit_epilogue) */
}