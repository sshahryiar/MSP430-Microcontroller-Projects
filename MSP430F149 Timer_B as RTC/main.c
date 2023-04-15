#include <msp430.h> 
#include "delay.h"
#include "lcd.h"


#define PM                      0
#define AM                      1

#define ETR                     1
#define INC                     2
#define DEC                     3
#define ESC                     4
#define NON                     0

#define ETR_button              !(P1IN & BIT3)
#define INC_button              !(P1IN & BIT2)
#define DEC_button              !(P1IN & BIT1)
#define ESC_button              !(P1IN & BIT0)


unsigned char dow = 1;
unsigned char s = 10;
unsigned char hr = 10;
unsigned char min = 10;
unsigned char date = 1;
unsigned char month = 1;
unsigned char toggle = 0;
unsigned char leap_year = 0;
unsigned char ampm = AM;

unsigned int year = 2000;


void GPIO_setup(void);
void BCSP_setup(void);
void Timer_B_setup(void);
void System_setup(void);
void WDTP_setup(void);
void LCD_print_value(unsigned char x_pos, unsigned char y_pos, unsigned char value);
void LCD_print_int_value(unsigned char x_pos, unsigned char y_pos, unsigned int value);
void display_time(void);
void AM_PM_disp(unsigned char state);
void dow_disp(unsigned char dow);
unsigned char get_buttons(void);
unsigned char set_value(unsigned char x_pos, unsigned char y_pos, signed char value, unsigned char max, unsigned char min, unsigned char type);
void setup_RTC(void);


#pragma vector = TIMERB1_VECTOR
__interrupt void TIMERB_ISR(void)
{
    toggle ^= 0x01;

    s++;

    if(s > 59)
    {
        s = 0;
        min++;

        if(min > 59)
        {
            min = 0;
            hr++;

            if(hr > 12)
            {
                hr = 1;
            }

            if((hr == 12) && (min == 0) && (s == 0))
            {
                ampm ^= 1;

                if(ampm == AM)
                {
                    date++;
                    dow++;
                }

                if(dow > 7)
                {
                    dow = 1;
                }

                if((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10) || (month == 12))
                {
                    if(date > 31)
                    {
                        date = 1;
                        month++;
                    }
                }

                else if((month == 4) || (month == 6) || (month == 9) || (month == 11))
                {
                    if(date > 30)
                    {
                        date = 1;
                        month++;
                    }
                }

                else
                {
                    if((year % 4) == 0)
                    {
                        if((year % 100) == 0)
                        {
                            if((year % 400) == 0)
                            {
                                leap_year = 1;
                            }

                            else
                            {
                                leap_year = 0;
                            }
                        }

                        else
                        {
                            leap_year = 1;
                        }
                    }

                    else
                    {
                        leap_year = 0;
                    }

                    if((leap_year) && (date > 29))
                    {
                         date = 1;
                         month++;
                    }

                    else if((!leap_year) && (date > 28))
                    {
                         date = 1;
                         month++;
                    }
                }

                if(month > 12)
                {
                    month = 1;
                    year++;
                }
            }
        }
    }

    TB0CTL &= ~TBIFG;
    TBIV &= ~TB0IV_TBIFG;
}


void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    GPIO_setup();
    BCSP_setup();
    Timer_B_setup();
    System_setup();
    WDTP_setup();

    LCD_init();
    LCD_clear_home();
	
    while(1)
    {
        if(get_buttons() == ESC)
        {
            while(get_buttons() == ESC);
            setup_RTC();
        }
        else
        {
            display_time();
        }
    };
}


void GPIO_setup(void)
{
    P1OUT = 0;
    P1DIR = 0;
    P1IES = 0;
    P1IFG = 0;
    P2OUT = 0;
    P2DIR = 0;
    P2IES = 0;
    P2IFG = 0;
    P3OUT = 0;
    P3DIR = 0;
    P4OUT = 0;
    P4DIR = BIT0 | BIT1 | BIT2;
    P5OUT = 0;
    P5DIR = BIT4 | BIT5 | BIT6 | BIT7;
    P6OUT = 0;
    P6DIR = 0;
}


void BCSP_setup(void)
{
    __delay_cycles(100000);

    BCSCTL1 &= ~XT2OFF;
    BCSCTL1 |= DIVA_0;
    BCSCTL2 = SELM_2 | DIVM_2 | DIVS_2 | SELS;
}


void Timer_B_setup(void)
{
    TBCCR0 = 32767;
    TBCTL = TBSSEL_1 | ID_0 | MC_1 | TBIE;
}


void System_setup(void)
{
    do
    {
        IFG1 &= ~OFIFG;

        __delay_cycles(100);

    } while (IFG1 & OFIFG);

    __bis_SR_register(GIE);
}


void WDTP_setup(void)
{
    WDTCTL = WDTPW | WDTHOLD;
}


void LCD_print_value(unsigned char x_pos, unsigned char y_pos, unsigned char value)
{
    LCD_goto(x_pos, y_pos);
    LCD_putchar((value / 10) + 0x30);
    LCD_goto((x_pos + 1), y_pos);
    LCD_putchar((value % 10) + 0x30);
}


void LCD_print_int_value(unsigned char x_pos, unsigned char y_pos, unsigned int value)
{
    unsigned char temp = 0x00;

    temp = (value / 100);
    LCD_print_value(x_pos, y_pos, temp);

    temp = (value % 100);
    LCD_print_value((x_pos + 2), y_pos, temp);

}


void display_time(void)
{
    LCD_print_value(2, 0, hr);
    LCD_print_value(5, 0, min);
    LCD_print_value(8, 0, s);

    switch(toggle)
    {
        case 1:
        {
            LCD_goto(4, 0);
            LCD_putchar(':');
            LCD_goto(7, 0);
            LCD_putchar(':');
            break;
        }
        default:
        {
            LCD_goto(4, 0);
            LCD_putchar(' ');
            LCD_goto(7, 0);
            LCD_putchar(' ');
            break;
        }
    }

    AM_PM_disp(ampm);
    dow_disp(dow);

    LCD_print_value(6, 1, date);

    LCD_goto(8, 1);
    LCD_putchar('.');
    LCD_print_value(9, 1, month);

    LCD_goto(11, 1);
    LCD_putchar('.');
    LCD_print_int_value(12, 1, year);
}


void dow_disp(unsigned char dow)
{
    LCD_goto(0, 1);

    switch(dow)
    {
      case 1:
      {
        LCD_putstr("MON");
        break;
      }
      case 2:
      {
        LCD_putstr("TUE");
        break;
      }
      case 3:
      {
        LCD_putstr("WED");
        break;
      }
      case 4:
      {
        LCD_putstr("THR");
        break;
      }
      case 5:
      {
        LCD_putstr("FRI");
        break;
      }
      case 6:
      {
        LCD_putstr("SAT");
        break;
      }
      case 7:
      {
        LCD_putstr("SUN");
        break;
      }
      default:
      {
        LCD_putstr("   ");
        break;
      }
    }
}


void AM_PM_disp(unsigned char state)
{
    LCD_goto(12, 0);

    switch(state)
    {
      case AM:
      {
        LCD_putstr("AM");
        break;
      }
      default:
      {
        LCD_putstr("PM");
        break;
      }
    }
}


unsigned char get_buttons(void)
{
    if(ETR_button)
    {
        return ETR;
    }

    else if(INC_button)
    {
        return INC;
    }

    else if(DEC_button)
    {
        return DEC;
    }

    else if(ESC_button)
    {
        return ESC;
    }

    else
    {
        return NON;
    }
}


unsigned char set_value(unsigned char x_pos, unsigned char y_pos, signed char value, unsigned char max, unsigned char min, unsigned char type)
{
    unsigned char tgl = 0;

    while(1)
    {
        tgl ^= 0x01;
        delay_ms(90);

        if(get_buttons() == INC)
        {
            value++;
        }

        if(value > max)
        {
            value = min;
        }

        if(get_buttons() == DEC)
        {
            value--;
        }

        if(value < min)
        {
            value = max;
        }

        switch(type)
        {
          case 1:
          {
            switch(tgl)
            {
                case 1:
                {
                    LCD_print_value(x_pos, y_pos, value);
                    break;
                }

                default:
                {
                    LCD_goto(x_pos, y_pos);
                    LCD_putstr("  ");
                    break;
                }
            }

            break;
          }

          case 2:
          {
            switch(tgl)
            {
                case 1:
                {
                    AM_PM_disp(value);
                    break;
                }

                default:
                {
                    LCD_goto(12, 0);
                    LCD_putstr("  ");
                    break;
                }
            }

            break;
          }

          default:
          {
              switch(tgl)
              {
                  case 1:
                  {
                      dow_disp(value);
                      break;
                  }
                  default:
                  {
                      dow_disp(0);
                      break;
                  }
              }
              break;
          }
        }

        if((get_buttons() == ETR) && (tgl == 1))
        {
           return value;
        }
    };
}


void setup_RTC(void)
{
    unsigned int yr1 = 0;
    unsigned int yr2 = 0;

    TBCTL = 0;

    yr1 = (year / 100);
    yr2 = (year % 100);

    hr = set_value(2, 0, hr, 12, 1, 1);
    delay_ms(200);
    min = set_value(5, 0, min, 59, 0, 1);
    delay_ms(200);
    s = set_value(8, 0, s, 59, 0, 1);
    delay_ms(200);
    ampm = set_value(12, 0, ampm, 1, 0, 2);
    delay_ms(200);
    dow = set_value(0, 1, dow, 7, 1, 0);
    delay_ms(200);
    date = set_value(6, 1, date, 31, 1, 1);
    delay_ms(200);
    month = set_value(9, 1, month, 12, 1, 1);
    delay_ms(200);
    yr1 = set_value(12, 1, yr1, 99, 0, 1);
    delay_ms(200);
    yr2 = set_value(14, 1, yr2, 99, 0, 1);
    delay_ms(200);

    year = ((yr1 * 100) + yr2);

    if((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10) || (month == 12))
    {
        if(date > 31)
        {
            date = 1;
        }
    }

    else if((month == 4) || (month == 6) || (month == 9) || (month == 11))
    {
        if(date > 30)
        {
            date = 1;
        }
    }

    else
    {
        if((year % 4) == 0)
        {
            if((year % 100) == 0)
            {
                if((year % 400) == 0)
                {
                    leap_year = 1;
                }

                else
                {
                    leap_year = 0;
                }
            }

            else
            {
                leap_year = 1;
            }
        }

        else
        {
            leap_year = 0;
        }

        if((leap_year) && (date > 29))
        {
             date = 1;
        }

        else if((!leap_year) && (date > 28))
        {
             date = 1;
        }
    }

    Timer_B_setup();
}
