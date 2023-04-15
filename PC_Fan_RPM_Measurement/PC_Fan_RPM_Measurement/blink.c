#include <msp430.h>
#include "delay.h"
#include "lcd.h"
#include "lcd_print.h"


unsigned int pulse_ticks = 0;
unsigned int start_time = 0;
unsigned int end_time = 0;


void GPIO_graceInit(void);
void BCSplus_graceInit(void);
void Timer1_A3_graceInit(void);
void ADC10_graceInit(void);
void System_graceInit(void);
void WDTplus_graceInit(void);
unsigned int get_ADC(unsigned int channel);


#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR_HOOK(void)
{
    if(TA1IV == TA1IV_TACCR1)
    {
        end_time = TA1CCR1;
        pulse_ticks = (end_time - start_time);
        start_time = end_time;
        TA1CCTL1 &= ~CCIFG;
    }
}


void main(void)
{
    unsigned long f = 0;
    unsigned long rpm = 0;
    float v = 0;

    /* Stop watchdog timer from timing out during initial start-up. */
    WDTCTL = WDTPW | WDTHOLD;

    /* initialize Config for the MSP430 GPIO */
    GPIO_graceInit();

    /* initialize Config for the MSP430 2xx family clock systems (BCS) */
    BCSplus_graceInit();

    /* initialize Config for the MSP430 A3 Timer0 */
    Timer1_A3_graceInit();

    /* initialize Config for the MSP430 10-bit Analog to Digital Converter (ADC) */
    ADC10_graceInit();

    /* initialize Config for the MSP430 System Registers */
    System_graceInit();

    /* initialize Config for the MSP430 WDT+ */
    WDTplus_graceInit();

    LCD_init();
    LCD_clear_home();

    LCD_goto(0, 1);
    LCD_putstr("Volt./ V:");

    while(1)
    {
        v = get_ADC(INCH_5);
        v *= 0.03226;

        f = (1000000.0 / (float)(pulse_ticks << 1));
        rpm = (f * 50);

        print_I(0, 0, f);
        LCD_goto(4, 0);
        LCD_putstr("Hz  RPM");
        print_I(11, 0, rpm);

        print_F(11, 1, v, 1);

        delay_ms(400);
    };
}


void GPIO_graceInit(void)
{
    /* USER CODE START (section: GPIO_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: GPIO_graceInit_prologue) */

    /* Port 1 Output Register */
    P1OUT = 0;

    /* Port 1 Direction Register */
    P1DIR = BIT6 | BIT7;

    /* Port 1 Interrupt Edge Select Register */
    P1IES = 0;

    /* Port 1 Interrupt Flag Register */
    P1IFG = 0;

    /* Port 2 Output Register */
    P2OUT = 0;

    /* Port 2 Port Select Register */
    P2SEL = BIT1;

    /* Port 2 Port Select Register */
    P2SEL &= ~(BIT6 | BIT7);

    /* Port 2 Direction Register */
    P2DIR = 0;

    /* Port 2 Interrupt Edge Select Register */
    P2IES = 0;

    /* Port 2 Interrupt Flag Register */
    P2IFG = 0;

    /* Port 3 Output Register */
    P3OUT = 0;

    /* Port 3 Direction Register */
    P3DIR = 0;

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

    if (CALBC1_1MHZ != 0xFF) {
        /* Follow recommended flow. First, clear all DCOx and MODx bits. Then
         * apply new RSELx values. Finally, apply new DCOx and MODx bit values.
         */
        DCOCTL = 0x00;
        BCSCTL1 = CALBC1_1MHZ;      /* Set DCO to 1MHz */
        DCOCTL = CALDCO_1MHZ;
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



void Timer1_A3_graceInit(void)
{
    /* USER CODE START (section: Timer1_A3_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: Timer1_A3_graceInit_prologue) */

    /*
     * TA1CCTL0, Capture/Compare Control Register 0
     *
     * CM_1 -- Rising Edge
     * CCIS_0 -- CCIxA
     * ~SCS -- Asynchronous Capture
     * ~SCCI -- Latched capture signal (read)
     * ~CAP -- Compare mode
     * OUTMOD_1 -- PWM output mode: 1 - Set
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    TA1CCTL0 = CM_1 | CCIS_0 | OUTMOD_1;

    /*
     * TA1CCTL1, Capture/Compare Control Register 1
     *
     * CM_1 -- Rising Edge
     * CCIS_0 -- CCIxA
     * SCS -- Sychronous Capture
     * ~SCCI -- Latched capture signal (read)
     * CAP -- Capture mode
     * OUTMOD_0 -- PWM output mode: 0 - OUT bit value
     *
     * Note: ~SCCI indicates that SCCI has value zero
     */
    TA1CCTL1 = CM_1 | CCIS_0 | SCS | CAP | OUTMOD_0 | CCIE;

    /*
     * TA1CTL, Timer_A3 Control Register
     *
     * TASSEL_2 -- SMCLK
     * ID_0 -- Divider - /1
     * MC_2 -- Continuous Mode
     */
    TA1CTL = TASSEL_2 | ID_0 | MC_2;

    /* USER CODE START (section: Timer1_A3_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: Timer1_A3_graceInit_epilogue) */
}


void ADC10_graceInit(void)
{
    /* USER CODE START (section: ADC10_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: ADC10_graceInit_prologue) */

    /* disable ADC10 during initialization */
    ADC10CTL0 &= ~ENC;

    /*
     * Control Register 0
     *
     * ~ADC10SC -- No conversion
     * ~ENC -- Disable ADC
     * ~ADC10IFG -- Clear ADC interrupt flag
     * ~ADC10IE -- Disable ADC interrupt
     * ADC10ON -- Switch On ADC10
     * ~REFON -- Disable ADC reference generator
     * ~REF2_5V -- Set reference voltage generator = 1.5V
     * ~MSC -- Disable multiple sample and conversion
     * ~REFBURST -- Reference buffer on continuously
     * ~REFOUT -- Reference output off
     * ~ADC10SR -- Reference buffer supports up to ~200 ksps
     * ADC10SHT_2 -- 16 x ADC10CLKs
     * SREF_0 -- VR+ = VCC and VR- = VSS
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    ADC10CTL0 = ADC10ON | ADC10SHT_2 | SREF_0;

    /*
     * Control Register 1
     *
     * ~ADC10BUSY -- No operation is active
     * CONSEQ_2 -- Repeat single channel
     * ADC10SSEL_0 -- ADC10OSC
     * ADC10DIV_0 -- Divide by 1
     * ~ISSH -- Input signal not inverted
     * ~ADC10DF -- ADC10 Data Format as binary
     * SHS_0 -- ADC10SC
     * INCH_5 -- ADC Channel 5
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    ADC10CTL1 = CONSEQ_2 | ADC10SSEL_0 | ADC10DIV_0 | SHS_0 | INCH_5;

    /* Analog (Input) Enable Control Register 0 */
    ADC10AE0 = 0x20;


    /* enable ADC10 */
    ADC10CTL0 |= ENC;

    /* USER CODE START (section: ADC10_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: ADC10_graceInit_epilogue) */
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


unsigned int get_ADC(unsigned int channel)
{
    ADC10CTL0 &= ~ENC;

    ADC10CTL1 &= ~(0xF000);
    ADC10CTL1 |= channel;

    ADC10CTL0 |= ENC;

    ADC10CTL0 |= ADC10SC;
    while ((ADC10CTL0 & ADC10IFG) == 0);

    return ADC10MEM;
}
