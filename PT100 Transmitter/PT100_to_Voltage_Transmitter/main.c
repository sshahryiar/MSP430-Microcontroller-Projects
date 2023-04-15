#include "driverlib.h"
#include "delay.h"
#include "ST7735.h"


/*
    signal (0 - 10V) ----/\/\/\---(.)----/\/\/\---- GND
                               R1     Vs       R2
*/

#define R1 2.2              // 2.2k ohms
#define R2 1.0              // 1.0k ohms
#define Vs ((R1 + R2) / R2)


void clock_init(void);
void GPIO_init(void);
void ADC12_init(void);
void DMA_HW_init(void);
void draw_background(void);
float map(float v, float x_min, float x_max, float y_min, float y_max);
float constrain(float value, float value_min, float value_max);


unsigned int memory_location = 0x0000;


#pragma vector = DMA_VECTOR
__interrupt void DMA_ISR (void)
{
    switch (__even_in_range(DMAIV, 16))
    {
        case 0: break;  //None
        case 2:         //DMA0IFG = DMA Channel 0
        {
            GPIO_toggleOutputOnPin(GPIO_PORT_P1,
                                   GPIO_PIN0);

            break;
        }
        case 4: break;  //DMA1IFG = DMA Channel 1
        case 6: break;  //DMA2IFG = DMA Channel 2
        case 8: break;  //DMA3IFG = DMA Channel 3
        case 10: break; //DMA4IFG = DMA Channel 4
        case 12: break; //DMA5IFG = DMA Channel 5
        case 14: break; //DMA6IFG = DMA Channel 6
        case 16: break; //DMA7IFG = DMA Channel 7
        default: break;
    }
}


void main(void)
{
    signed char bar = 0;
    signed char samples = 0;

    unsigned int avg = 0;
    unsigned int adc = 0;

    float volt = 0.0;
    float t_present = 0.0;
    float t_previous = 0.0;

    WDT_A_hold(WDT_A_BASE);

    clock_init();
    GPIO_init();
    ADC12_init();
    DMA_HW_init();

    ST7735_init();
    draw_background();

    while(1)
    {
        adc += memory_location;
        samples++;

        if(samples >= 16)
        {
            avg = (adc >> 4);

            volt = (((float)avg * 3.3) / 4095.0);
            volt *= Vs;

            t_present = ((15.0 * volt) - 50.0);

            t_present = constrain(t_present, -50, 100);

            if(t_present != t_previous)
            {
                bar = map(t_present, -50, 100, 136, 36);

                Draw_Rectangle(12, 36, 14, bar, YES, ROUND, BLACK, BLACK);
                Draw_Rectangle(12, 136, 14, bar, YES, ROUND, RED, BLACK);

                print_I(58, 65, 2, BLUE, BLACK, (volt * 1000));
                print_F(58, 125, 2, GREEN, BLACK, t_present, 1);

                t_previous = t_present;
            }

            avg = 0;
            adc = 0;
            samples = 0;
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
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                                               GPIO_PIN0);

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

    configureMemoryParam.memoryBufferControlIndex = ADC12_A_MEMORY_0;
    configureMemoryParam.inputSourceSelect = ADC12_A_INPUT_A0;
    configureMemoryParam.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
    configureMemoryParam.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
    configureMemoryParam.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;

    ADC12_A_configureMemory(ADC12_A_BASE,
                            &configureMemoryParam);

    ADC12_A_enable(ADC12_A_BASE);

    ADC12_A_startConversion(ADC12_A_BASE,
                            ADC12_A_MEMORY_0,
                            ADC12_A_REPEATED_SINGLECHANNEL);
}


void DMA_HW_init(void)
{
    DMA_initParam DMA_init_Param = {0};

    DMA_disableTransferDuringReadModifyWrite();

    DMA_init_Param.channelSelect = DMA_CHANNEL_0;
    DMA_init_Param.transferModeSelect = DMA_TRANSFER_REPEATED_SINGLE;
    DMA_init_Param.transferSize = 1;
    DMA_init_Param.triggerSourceSelect = DMA_TRIGGERSOURCE_24;
    DMA_init_Param.transferUnitSelect = DMA_SIZE_SRCWORD_DSTWORD;
    DMA_init_Param.triggerTypeSelect = DMA_TRIGGER_RISINGEDGE;

    DMA_init(&DMA_init_Param);

    DMA_setSrcAddress(DMA_CHANNEL_0,
                      ADC12_A_getMemoryAddressForDMA(ADC12_A_BASE,ADC12_A_MEMORY_0),
                      DMA_DIRECTION_UNCHANGED);

    DMA_setDstAddress(DMA_CHANNEL_0,
                      (uint32_t)(uintptr_t)&memory_location,
                      DMA_DIRECTION_INCREMENT);

    DMA_clearInterrupt(DMA_CHANNEL_0);

    DMA_enableInterrupt(DMA_CHANNEL_0);

    DMA_enableTransfers(DMA_CHANNEL_0);

    __enable_interrupt();
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
        print_C(32, (i + 36), 1, YELLOW, BLACK, (100 - (i*1.5)));
    }

    print_str(1, 4, 1, CYAN, BLACK, "PT100 - V Transmitter");
    print_str(64, 45, 2, BLUE, BLACK, "V/mV");
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
