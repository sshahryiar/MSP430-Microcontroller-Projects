#include <msp430.h>
#include "delay.h"
#include "lcd.h"
#include "lcd_print.h"
#include "ToF10120.h"


void setup(void);
void GPIO_graceInit(void);
void BCSplus_graceInit(void);
void USI_graceInit(void);
void System_graceInit(void);
void WDTplus_graceInit(void);


void main(void)
{
    setup();

    LCD_goto(1, 0);
    LCD_putstr("ToF10120 Demo.");
    LCD_goto(0, 1);
    LCD_putstr("Range/mm:");

    while(1)
    {
        P1OUT ^= BIT0;
        print_I(11, 1, ToF10120_get_distance());
        delay_ms(60);
    };
}


void setup(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    GPIO_graceInit();
    BCSplus_graceInit();
    USI_graceInit();
    System_graceInit();
    WDTplus_graceInit();

    ToF10120_init();
    LCD_init();
    LCD_clear_home();
}


void GPIO_graceInit(void)
{
    /* USER CODE START (section: GPIO_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: GPIO_graceInit_prologue) */

    /* Port 1 Output Register */
    P1OUT = 0;

    /* Port 1 Port Select Register */
    P1SEL = BIT6 | BIT7;

    /* Port 1 Direction Register */
    P1DIR = BIT0;

    /* Port 1 Interrupt Edge Select Register */
    P1IES = 0;

    /* Port 1 Interrupt Flag Register */
    P1IFG = 0;

    /* Port 2 Output Register */
    P2OUT = 0;

    /* Port 2 Port Select Register */
    P2SEL &= ~(BIT6 | BIT7);

    /* Port 2 Direction Register */
    P2DIR = 0;

    /* Port 2 Interrupt Edge Select Register */
    P2IES = 0;

    /* Port 2 Interrupt Flag Register */
    P2IFG = 0;

    /* USER CODE START (section: GPIO_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: GPIO_graceInit_epilogue) */
}


void BCSplus_graceInit(void)
{
    /* USER CODE START (section: BCSplus_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: BCSplus_graceInit_prologue) */

    /*
     * Basic Clock System Control 2
     *
     * SELM_0 -- DCOCLK
     * DIVM_0 -- Divide by 1
     * ~SELS -- DCOCLK
     * DIVS_0 -- Divide by 1
     * ~DCOR -- DCO uses internal resistor
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    BCSCTL2 = SELM_0 | DIVM_0 | DIVS_0;

    if (CALBC1_8MHZ != 0xFF) {
        /* Adjust this accordingly to your VCC rise time */
        __delay_cycles(100000);

        // Follow recommended flow. First, clear all DCOx and MODx bits. Then
        // apply new RSELx values. Finally, apply new DCOx and MODx bit values.
        DCOCTL = 0x00;
        BCSCTL1 = CALBC1_8MHZ;      /* Set DCO to 8MHz */
        DCOCTL = CALDCO_8MHZ;
    }

    /*
     * Basic Clock System Control 1
     *
     * XT2OFF -- Disable XT2CLK
     * ~XTS -- Low Frequency
     * DIVA_0 -- Divide by 1
     *
     * Note: ~XTS indicates that XTS has value zero
     */
    BCSCTL1 |= XT2OFF | DIVA_0;

    /*
     * Basic Clock System Control 3
     *
     * XT2S_0 -- 0.4 - 1 MHz
     * LFXT1S_2 -- If XTS = 0, XT1 = VLOCLK ; If XTS = 1, XT1 = 3 - 16-MHz crystal or resonator
     * XCAP_1 -- ~6 pF
     */
    BCSCTL3 = XT2S_0 | LFXT1S_2 | XCAP_1;

    /* USER CODE START (section: BCSplus_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: BCSplus_graceInit_epilogue) */
}


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


void System_graceInit(void)
{
    /* USER CODE START (section: System_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: System_graceInit_prologue) */

    /* Clear oscillator fault flag with software delay */
    do
    {
        // Clear OSC fault flag
        IFG1 &= ~OFIFG;

        // 50us delay
        __delay_cycles(50);
    } while (IFG1 & OFIFG);

    /*
     * SR, Status Register
     *
     * ~SCG1 -- Disable System clock generator 1
     * ~SCG0 -- Disable System clock generator 0
     * ~OSCOFF -- Oscillator On
     * ~CPUOFF -- CPU On
     * GIE -- General interrupt enable
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    __bis_SR_register(GIE);

    /* USER CODE START (section: System_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: System_graceInit_epilogue) */
}


void WDTplus_graceInit(void)
{
    /* USER CODE START (section: RTC_B_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: RTC_B_graceInit_prologue) */

    /*
     * WDTCTL, Watchdog Timer+ Register
     *
     * WDTPW -- Watchdog password
     * WDTHOLD -- Watchdog timer+ is stopped
     * ~WDTNMIES -- NMI on rising edge
     * ~WDTNMI -- Reset function
     * ~WDTTMSEL -- Watchdog mode
     * ~WDTCNTCL -- No action
     * ~WDTSSEL -- SMCLK
     * ~WDTIS0 -- Watchdog clock source bit0 disabled
     * ~WDTIS1 -- Watchdog clock source bit1 disabled
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    WDTCTL = WDTPW | WDTHOLD;

    /* USER CODE START (section: RTC_B_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: RTC_B_graceInit_epilogue) */
}

