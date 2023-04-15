#include "driverlib.h"
#include "delay.h"
#include "ST7735.h"
#include "math.h"

#define PI              3.142
#define deg_to_rad      (PI / 180.0)
#define INC             1
#define DEC             0
#define deg_max         1500
#define deg_min         0
#define deg_change      10


unsigned long cnt = 0;


void clock_init(void);
void GPIO_init(void);
void timer_T2A3_init(void);
void ADC12_init(void);
void draw_background(void);
float map(float v, float x_min, float x_max, float y_min, float y_max);
float constrain(float value, float valbue_min, float value_max);


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
    unsigned char dir = INC;
    unsigned int deg = 0;
    unsigned int r_max = 0;
    unsigned int r_min = 255;
    signed int x1 = 0;
    signed int x2 = 0;
    signed int y1 = 0;
    signed int y2 = 0;

    float l = 0.0;
    float r_in = 0.0;
    float degrees = 0.0;

    WDT_A_hold(WDT_A_BASE);

    clock_init();
    GPIO_init();
    ADC12_init();
    timer_T2A3_init();

    ST7735_init();
    TFT_fill(BLACK);
    draw_background();

    print_str(17, 1, 1, CYAN, BLACK, "LV-MaxSONAR EZ3 SONAR");
    print_str(1, 10, 1, WHITE, BLACK, "Target./in:");
    print_str(1, 18, 1, WHITE, BLACK, "R. Max./in:");
    print_str(1, 26, 1, WHITE, BLACK, "R. Min./in:");
    print_str(1, 34, 1, YELLOW, BLACK, "Sweep./deg:");

    while(1)
    {
        GPIO_toggleOutputOnPin(GPIO_PORT_P1,
                               GPIO_PIN0);

        if((deg < deg_max) && (dir == INC))
        {
            deg += deg_change;
        }

        if((deg == deg_max) && (dir == INC))
        {
            dir = DEC;
            r_max = 0;
            r_min = 255;
        }

        if((deg > deg_min) && (dir == DEC))
        {
            deg -= deg_change;
        }

        if((deg == deg_min) && (dir == DEC))
        {
            dir = INC;
            r_max = 0;
            r_min = 255;
        }

        r_in = map((float)cnt, 0.0, 2010.0, 0.0, 254.0);

        if(r_in > r_max)
        {
            r_max = r_in;
        }

        if(r_in < r_min)
        {
            r_min = r_in;
        }

        l = map(r_in, 0.0, 254.0, 0.0, 75.0);
        l = constrain(l, 0.0, 75.0);

        draw_background();

        degrees = map(((float)deg), deg_min, deg_max, 0.0, 180.0);

        x1 = (80 + (75 * (cos(deg_to_rad * degrees))));
        y1 = (127 - (75 * (sin(deg_to_rad * degrees))));
        x2 = (80 + (l * (cos(deg_to_rad * degrees))));
        y2 = (127 - (l * (sin(deg_to_rad * degrees))));

        Draw_Line(80, 127, x1, y1, BLACK);
        Draw_Line(80, 127, x2, y2, RED);

        print_I(72, 10, 1, WHITE, BLACK, r_in);
        print_I(72, 18, 1, WHITE, BLACK, r_max);
        print_I(72, 26, 1, WHITE, BLACK, r_min);
        print_I(72, 34, 1, YELLOW, BLACK, degrees);

        Timer_A_setCompareValue(TIMER_A2_BASE,
                               TIMER_A_CAPTURECOMPARE_REGISTER_1,
                               (1000 + deg));

        delay_ms(1);
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

    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2,
                                                GPIO_PIN4);
}


void timer_T2A3_init(void)
{
    Timer_A_outputPWMParam outputPWMParam = {0};

    outputPWMParam.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    outputPWMParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_2;
    outputPWMParam.timerPeriod = 20000;
    outputPWMParam.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    outputPWMParam.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
    outputPWMParam.dutyCycle = 0;

    Timer_A_outputPWM(TIMER_A2_BASE,
                      &outputPWMParam);

    Timer_A_setCompareValue(TIMER_A2_BASE,
                            TIMER_A_CAPTURECOMPARE_REGISTER_1,
                            1000);
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
    signed int i = 180;

    Draw_Circle(80, 127, 76, NO, GREEN);
    Draw_Circle(80, 127, 78, NO, GREEN);

    Draw_Circle(80, 127, 25, NO, GREEN);
    Draw_Circle(80, 127, 50, NO, GREEN);

    while(i >= -1)
    {
        Draw_Line(80, 127, (80 + (75 * cos((float)i * deg_to_rad))),  (127 - (75 * sin((float)i * deg_to_rad))), GREEN);
        i -= 30;
    }
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
