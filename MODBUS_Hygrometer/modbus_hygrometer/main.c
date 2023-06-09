#include <msp430.h> 
#include "delay.h"
#include "lcd.h"
#include "lcd_print.h"


#define REDE_PORT                               P3OUT

#define DIR_RX                                  do{REDE_PORT &= ~BIT3;}while(0)
#define DIR_TX                                  do{REDE_PORT |= BIT3;}while(0)

#define TX_buffer_length                        10
#define RX_buffer_length                        10
#define fixed_no_of_bytes_to_read               5


unsigned char cnt = 0;
unsigned char RX_buffer[RX_buffer_length];


void setup(void);
void load_degree_symbol(unsigned char y_pos, unsigned char x_pos);
void flush_RX_buffer(void);
void send_read_command(void);
unsigned int make_word(unsigned char HB, unsigned char LB);
void get_HB_LB(unsigned int value, unsigned char *HB, unsigned char *LB);
unsigned int MODBUS_RTU_CRC16(unsigned char *data_input, unsigned char data_length);


#pragma vector = USART1RX_VECTOR
__interrupt void USART_RX_ISR_HOOK(void)
{
    RX_buffer[cnt++] = U1RXBUF;
}


void main(void)
{
    unsigned int value = 0;
    unsigned int CRC_check_1 = 0;
    unsigned int CRC_check_2 = 0;

    setup();

    LCD_goto(0, 0);
    LCD_putstr("R.H / %:");
    LCD_goto(0, 1);
    LCD_putstr("Temp/ C:");
    print_symbol(5, 1, 0);
	
	while(1)
	{
	    send_read_command();

	    CRC_check_1 = MODBUS_RTU_CRC16(RX_buffer, 7);
	    CRC_check_2 = make_word(RX_buffer[8], RX_buffer[7]);

        if(CRC_check_1 == CRC_check_2)
        {
             value = make_word(RX_buffer[5], RX_buffer[6]);
             print_F(11, 0, (value / 10.0), 1);

             value = make_word(RX_buffer[3], RX_buffer[4]);
             print_F(11, 1, (value / 10.0), 1);

        }

        else
        {
            LCD_goto(12, 0);
            LCD_putstr("--.-");
            LCD_goto(12, 1);
            LCD_putstr("--.-");
        }

        delay_ms(400);
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
    P3SEL = (BIT6 | BIT7);
    P3DIR = BIT3;
    P4OUT = 0;
    P4DIR = BIT0 | BIT1 | BIT2;
    P5OUT = 0;
    P5DIR = BIT4 | BIT5 | BIT6 | BIT7;
    P6OUT = 0;
    P6DIR = 0;

    U1CTL = SWRST;
    U1CTL &= ~SWRST;
    U1CTL |= CHAR;
    ME2 = (URXE1 | UTXE1);
    U1TCTL |= SSEL0;
    U1BR1 = 0x00;
    U1BR0 = 0x03;
    U1MCTL = 0x4A;
    IE2 |= URXIE1;

    flush_RX_buffer();

    LCD_init();
    LCD_clear_home();
    load_custom_symbol();

    _BIS_SR(GIE);
}


void flush_RX_buffer(void)
{
    signed char i = (RX_buffer_length - 1);

    while(i > -1)
    {
        RX_buffer[i] = 0x00;
        i--;
    };
}


void send_read_command(void)
{
    unsigned char i = 0;

    volatile unsigned char TX_buffer[TX_buffer_length] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x0B};

    flush_RX_buffer();

    DIR_TX;

    for(i = 0; i < TX_buffer_length; i++)
    {
        U1TXBUF = TX_buffer[i];
        while(!(IFG2 & UTXIFG1));
    }

    cnt = 0;
    DIR_RX;

    delay_ms(600);
}


unsigned int make_word(unsigned char HB, unsigned char LB)
{
    unsigned int tmp = 0;

    tmp = HB;
    tmp <<= 8;
    tmp |= LB;

    return tmp;
}


void get_HB_LB(unsigned int value, unsigned char *HB, unsigned char *LB)
{
    *LB = (value & 0x00FF);
    *HB = ((value & 0xFF00) >> 8);
}


unsigned int MODBUS_RTU_CRC16(unsigned char *data_input, unsigned char data_length)
{
    static const unsigned int CRC_table[256] =
    {
       0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
       0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
       0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
       0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
       0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
       0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
       0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
       0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
       0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
       0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
       0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
       0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
       0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
       0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
       0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
       0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
       0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
       0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
       0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
       0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
       0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
       0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
       0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
       0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
       0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
       0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
       0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
       0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
       0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
       0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
       0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
       0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
     };

    unsigned char temp = 0;
    unsigned int CRC_word = 0xFFFF;

    while(data_length--)
    {
        temp = *data_input++ ^ CRC_word;
        CRC_word >>= 8;
        CRC_word ^= CRC_table[temp];
    }

    return CRC_word;
}
