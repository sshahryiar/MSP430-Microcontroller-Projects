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
 *      D:/ti/grace/grace_2_20_02_32/packages/ti/mcu/msp430/csl/interrupt_vectors/InterruptVectors_init.xdt
 */
#include <msp430.h>

/* USER CODE START (section: InterruptVectors_init_c_prologue) */
/* User defined includes, defines, global variables and functions */
/* USER CODE END (section: InterruptVectors_init_c_prologue) */

/*
 *  ======== InterruptVectors_graceInit ========
 */
void InterruptVectors_graceInit(void)
{
}


/* Interrupt Function Prototypes */


/*
 *  ======== USI Interrupt Service Routine ========
 *
 *  You can check which interrupt flag was triggered here.
 *  Copy and paste the following function as desired.
 *
    if (USICTL1 & USIIFG) {

        return;
    }
    if (USICTL1 & USISTTIFG) {

        return;
    }
 */
#pragma vector=USI_VECTOR
__interrupt void USI_ISR_HOOK(void)
{
    /* USER CODE START (section: USI_ISR_HOOK) */
    /* replace this comment with your code */
    /* USER CODE END (section: USI_ISR_HOOK) */
}
