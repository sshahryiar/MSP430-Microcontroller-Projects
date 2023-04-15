#include <msp430.h> 
#include "delay.h"
#include "lcd.h"
#include "lcd_print.h"
#include "NRF905.h"
#include "DS18B20.h"



void setup(void);


void main(void)
{
    unsigned char hb = 0x00;
    unsigned char lb = 0x00;
    signed int tmp = 0x0000;
    float temp = 0.0;

    NRF905_TX_buffer[0] = 0x99;
    NRF905_TX_buffer[31] = 0x66;

    setup();

    LCD_goto(0, 0);
    LCD_putstr("nRF905 DS18B20 T");
    LCD_goto(0, 1);
    LCD_putstr("Temp/ C:");
    print_symbol(5, 1, 0);

    while(1)
    {
        tmp = DS18B20_get_raw_temperature();
        temp = DS18B20_get_temperature(tmp);

        lb = (tmp & 0x00FF);
        hb = ((tmp & 0xFF00) >> 8);

        NRF905_TX_buffer[1] = lb;
        NRF905_TX_buffer[2] = hb;

        print_F(11, 1, temp, 1);

        NRF905_TX();
        delay_ms(60);
    };
}


void setup(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    BCSCTL2 = SELM_2 | DIVM_0 | SELS | DIVS_0;
    BCSCTL1 |= DIVA_0;
    BCSCTL1 &= ~XT2OFF;

    P1OUT = 0;
    P1DIR = 0;
    P1IES = 0;
    P1IFG = 0;
    P2OUT = 0;
    P2DIR = 0;
    P2IES = 0;
    P2IFG = 0;
    P3OUT = 0;
    P3SEL = 0;
    P3DIR = 0;
    P4OUT = 0;
    P4DIR = BIT0 | BIT1 | BIT2;
    P5OUT = 0;
    P5DIR = BIT4 | BIT5 | BIT6 | BIT7;
    P6OUT = 0;
    P6DIR = 0;

    LCD_init();
    LCD_clear_home();
    load_custom_symbol();

    NRF905_init();
}


