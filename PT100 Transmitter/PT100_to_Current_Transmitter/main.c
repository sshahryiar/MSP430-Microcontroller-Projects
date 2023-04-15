#include "driverlib.h"
#include "delay.h"
#include "ST7735.h"


/*
    (-)---(.)----/\/\/\---- GND
    T-Ve   Vs       RS
*/


#define Rs 150.0 //150 ohms sampling shunt resistor


unsigned long cnt = 0;


void clock_init(void);
void GPIO_init(void);
void ADC12_init(void);
void draw_background(void);
float map(float v, float x_min, float x_max, float y_min, float y_max);
float constrain(float value, float value_min, float value_max);



#pragma vector = ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
    switch (__even_in_range(ADC12IV, 34))
    {
        case  0: break;   //Vector  0:  No interrupt
        case  2: break;   //Vector  2:  ADC overflow
        case  4: break;   //Vector  4:  ADC timing overflow
        case  6: break;   //Vector  6:  ADC12IFG0
        case  8:
        {
             cnt = ADC12_A_getResults(ADC12_A_BASE,
                                      ADC12_A_MEMORY_1);



             break;             //Vector  8:  ADC12IFG1
        }
        case 10: break;   //Vector 10:  ADC12IFG2
        case 12: break;   //Vector 12:  ADC12IFG3
        case 14: break;   //Vector 14:  ADC12IFG4
        case 16: break;   //Vector 16:  ADC12IFG5
        case 18: break;   //Vector 18:  ADC12IFG6
        case 20: break;   //Vector 20:  ADC12IFG7
        case 22: break;   //Vector 22:  ADC12IFG8
        case 24: break;   //Vector 24:  ADC12IFG9
        case 26: break;   //Vector 26:  ADC12IFG10
        case 28: break;   //Vector 28:  ADC12IFG11
        case 30: break;   //Vector 30:  ADC12IFG12
        case 32: break;   //Vector 32:  ADC12IFG13
        case 34: break;   //Vector 34:  ADC12IFG14
        default: break;
    }
}


void main(void)
{
    signed char bar = 0;

    signed char samples = 0;

    unsigned long avg = 0;
    unsigned long adc = 0;

    float current = 0.0;
    float t_present = 0.0;
    float t_previous = 0.0;

    WDT_A_hold(WDT_A_BASE);

    clock_init();
    GPIO_init();
    ADC12_init();

    ST7735_init();
    draw_background();

    while(1)
    {
        adc += cnt;
        samples++;

        if(samples >= 16)
        {
            GPIO_toggleOutputOnPin(GPIO_PORT_P1,
                                   GPIO_PIN0);

            avg = (adc >> 4);

            adc = 0;
            samples = 0;

            current = ((((float)avg * 3300.0) / 4095.0) / Rs);

            if(current <= 2)
            {
                current = 0;
                Draw_Rectangle(12, 36, 14, 136, YES, ROUND, BLACK, BLACK);

                print_I(58, 65, 2, BLUE, BLACK, 0.0);
                print_str(54, 120, 2, RED, BLACK, "OPEN!  ");
                delay_ms(600);
                print_str(54, 120, 2, BLACK, BLACK, "       ");
                delay_ms(400);
            }

            else
            {
                t_present = ((3.125 * current) - 12.5);
                t_present = constrain(t_present, 0, 50 );

                if(t_present != t_previous)
                {
                    bar = map(t_present, 0, 50, 136, 36);

                    Draw_Rectangle(12, 36, 14, bar, YES, ROUND, BLACK, BLACK);
                    Draw_Rectangle(12, 136, 14, bar, YES, ROUND, RED, BLACK);

                    print_F(54, 65, 2, BLUE, BLACK, current, 2);
                    print_F(54, 120, 2, GREEN, BLACK, t_present, 1);

                    t_previous = t_present;
                }
            }
        }

        GPIO_toggleOutputOnPin(GPIO_PORT_P4,
                               GPIO_PIN7);
        delay_ms(250);
    };
}


void clock_init(void)
{
    PMM_setVCore(PMM_CORE_LEVEL_3);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,
                                               (GPIO_PIN4 | GPIO_PIN2));

    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5,
                                                (GPIO_PIN5 | GPIO_PIN3));

    UCS_setExternalClockSource(XT1_FREQ,
                               XT2_FREQ);

    UCS_turnOnXT2(UCS_XT2_DRIVE_4MHZ_8MHZ);

    UCS_turnOnLFXT1(UCS_XT1_DRIVE_0,
                    UCS_XCAP_3);

    UCS_initClockSignal(UCS_FLLREF,
                        UCS_XT2CLK_SELECT,
                        UCS_CLOCK_DIVIDER_4);

    UCS_initFLLSettle(MCLK_KHZ,
                      MCLK_FLLREF_RATIO);

    UCS_initClockSignal(UCS_SMCLK,
                        UCS_XT2CLK_SELECT,
                        UCS_CLOCK_DIVIDER_2);

    UCS_initClockSignal(UCS_ACLK,
                        UCS_XT1CLK_SELECT,
                        UCS_CLOCK_DIVIDER_1);
}


void GPIO_init(void)
{
    GPIO_setAsOutputPin(GPIO_PORT_P1,
                            GPIO_PIN0);

    GPIO_setDriveStrength(GPIO_PORT_P1,
                          GPIO_PIN0,
                          GPIO_FULL_OUTPUT_DRIVE_STRENGTH);

    GPIO_setAsOutputPin(GPIO_PORT_P4,
                        GPIO_PIN7);

    GPIO_setDriveStrength(GPIO_PORT_P4,
                          GPIO_PIN7,
                          GPIO_FULL_OUTPUT_DRIVE_STRENGTH);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                                               GPIO_PIN1);
}



void ADC12_init(void)
{
    ADC12_A_configureMemoryParam configureMemoryParam = {0};

    ADC12_A_init(ADC12_A_BASE,
                 ADC12_A_SAMPLEHOLDSOURCE_SC,
                 ADC12_A_CLOCKSOURCE_ACLK,
                 ADC12_A_CLOCKDIVIDER_1);

    ADC12_A_setupSamplingTimer(ADC12_A_BASE,
                               ADC12_A_CYCLEHOLD_768_CYCLES,
                               ADC12_A_CYCLEHOLD_4_CYCLES,
                               ADC12_A_MULTIPLESAMPLESENABLE);

    ADC12_A_setResolution(ADC12_A_BASE,
                          ADC12_A_RESOLUTION_12BIT);

    configureMemoryParam.memoryBufferControlIndex = ADC12_A_MEMORY_1;
    configureMemoryParam.inputSourceSelect = ADC12_A_INPUT_A1;
    configureMemoryParam.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
    configureMemoryParam.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
    configureMemoryParam.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;

    ADC12_A_configureMemory(ADC12_A_BASE,
                            &configureMemoryParam);

    ADC12_A_clearInterrupt(ADC12_A_BASE,
                           ADC12IFG0);

    ADC12_A_enableInterrupt(ADC12_A_BASE,
                            ADC12IE1);

    __enable_interrupt();

    ADC12_A_enable(ADC12_A_BASE);

    ADC12_A_startConversion(ADC12_A_BASE,
                            ADC12_A_MEMORY_1,
                            ADC12_A_REPEATED_SINGLECHANNEL);
}


void draw_background(void)
{
    unsigned char i = 0x00;

    TFT_fill(BLACK);
    Draw_Circle(13, 145, 9, NO, WHITE);
    Draw_Line(11, 139, 15, 139, BLACK);
    Draw_Line(10, 137, 10, 34, WHITE);
    Draw_Line(16, 137, 16, 34, WHITE);
    Draw_Line(11, 33, 15, 33, WHITE);
    Draw_Line(12, 33, 14, 33, BLACK);
    Draw_Line(12, 32, 14, 32, WHITE);
    Draw_Circle(13, 145, 7, YES, RED);
    Draw_Rectangle(12, 137, 14, 138, YES, SQUARE, RED, RED);

    for(i = 0; i <= 100; i += 10)
    {
        Draw_Line(24, (36 + i), 28, (36 + i), YELLOW);
        print_C(32, (i + 36), 1, YELLOW, BLACK, (50 - (i / 2.0)));
    }

    print_str(1, 4, 1, CYAN, BLACK, "PT100 - I Transmitter");

    print_str(64, 45, 2, BLUE, BLACK, "I/mA");
    print_str(64, 105, 2, GREEN, BLACK, "T/ C");

    Draw_Circle(93, 105, 3, NO, GREEN);
}


float map(float v, float x_min, float x_max, float y_min, float y_max)
{
    return (y_min + (((y_max - y_min)/(x_max - x_min)) * (v - x_min)));
}


float constrain(float value, float value_min, float value_max)
{
      if(value >= value_max)
      {
           return value_max;
      }
      else if(value <= value_min)
      {
           return value_min;
      }
      else
      {
           return value;
      }
}
