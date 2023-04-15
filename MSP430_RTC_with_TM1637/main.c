#include <msp430.h> 
#include "delay.h"
#include "TM1637.h"


#define PM                      0
#define AM                      1

#define ETR_button              !(P1IN & BIT0)
#define INC_button              !(P1IN & BIT1)
#define DEC_button              !(P1IN & BIT2)
#define ESC_button              !(P1IN & BIT3)

#define T_value                 32768


unsigned char dow = 1;
unsigned char s = 10;
unsigned char hr = 10;
unsigned char min = 10;
unsigned char date = 1;
unsigned char month = 1;
unsigned char toggle = 0;
unsigned char leap_year = 0;
unsigned char ampm = AM;

unsigned int year = 0;


extern const unsigned char seg_data[10];


void setup(void);
unsigned char inc_dec(signed char value, signed char value_max, signed char value_min);


#pragma vector = TIMERA1_VECTOR
__interrupt void TIMERA1_ISR_HOOK(void)
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

    P5OUT ^= BIT0;
    TACTL &= ~TAIFG;

}


void main(void)
{
    unsigned char set = 0x00;
    unsigned char time_menu = 0x00;
    unsigned char auto_scroll = 0x00;
    unsigned char toggle = 0x00;

    setup();


    while(1)
    {
      if(ETR_button)
      {
          delay_ms(40);

          while(ETR_button);

          if(set == 1)
          {
              time_menu++;
          }

          else
          {
              time_menu = 1;
              auto_scroll = 0;
          }

          set = 1;
          toggle = 1;

          TACTL &= ~TAIE;
          __disable_interrupt();

          P5OUT |= BIT0;
          TM1637_clear();
      }

      if(ESC_button)
      {
          while(ESC_button);
          auto_scroll ^= 1;
      }

      switch(set)
      {
          case 1:
          {
                switch(time_menu)
                {
                    case 1:
                    {
                        TM1637_display_segments(0, 0x00, 0);
                        TM1637_display_segments(1, 0x00, 1);

                        hr = inc_dec(hr, 23, 0);
                        break;
                    }

                    case 2:
                    {
                        TM1637_display_segments(2, 0x00, 0);
                        TM1637_display_segments(3, 0x00, 0);

                        min = inc_dec(min, 59, 0);
                        break;
                    }

                    case 3:
                    {
                        TM1637_display_segments(0, 0x00, 0);
                        TM1637_display_segments(1, 0x00, 0);

                        date = inc_dec(date, 31, 1);
                        break;
                    }

                    case 4:
                    {
                        TM1637_display_segments(2, 0x00, 0);
                        TM1637_display_segments(3, 0x00, 0);

                        month = inc_dec(month, 12, 1);
                        break;
                    }

                    case 5:
                    {
                        TM1637_display_segments(0, seg_data[2], 0);
                        TM1637_display_segments(1, seg_data[0], 0);
                        TM1637_display_segments(2, 0x00, 0);
                        TM1637_display_segments(3, 0x00, 0);

                        year = inc_dec(year, 99, 0);
                        break;
                    }

                    default:
                    {
                        time_menu = 0;
                        set = 0;
                        toggle = 1;
                        break;
                    }
                }

                TACTL |= TAIE;
                __enable_interrupt();

                break;
          }

          default:
          {
              toggle ^= 1;

              if(INC_button)
              {
                  delay_ms(100);
                  time_menu++;
              }

              if(DEC_button)
              {
                  delay_ms(100);
                  time_menu--;
              }

              switch(auto_scroll)
              {
                  case 1:
                  {
                    time_menu++;
                    delay_ms(600);
                    break;
                  }

                  default:
                  {
                      //time_menu = 1;
                      break;
                  }
              }

              delay_ms(100);
              break;
          }
      }


      switch(time_menu)
      {
            case 1:

            case 2:
            {
                TM1637_display_segments(0, seg_data[(hr / 10)], 0);
                TM1637_display_segments(1, seg_data[(hr % 10)], toggle);
                TM1637_display_segments(2, seg_data[(min / 10)], 0);
                TM1637_display_segments(3, seg_data[(min % 10)], 0);
                break;
            }

            case 3:

            case 4:
            {
                  TM1637_display_segments(0, seg_data[(date / 10)], 0);
                  TM1637_display_segments(1, seg_data[(date % 10)], 0);
                  TM1637_display_segments(2, seg_data[(month / 10)], 0);
                  TM1637_display_segments(3, seg_data[(month % 10)], 0);
                  break;
            }

            case 5:
            {
                  TM1637_display_segments(0, seg_data[2], 0);
                  TM1637_display_segments(1, seg_data[0], 0);
                  TM1637_display_segments(2, seg_data[(year / 10)], 0);
                  TM1637_display_segments(3, seg_data[(year % 10)], 0);
                  break;
            }

            default:
            {
                time_menu = 1;
                break;
            }
      }

    }
}


void setup(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    BCSCTL1 &= ~XT2OFF;
    BCSCTL1 |= DIVA_0;
    BCSCTL2 = SELM_2 | DIVM_0 | SELS | DIVS_0;

    P1OUT = 0;
    P1DIR = 0;
    P1IES = 0;
    P1IFG = 0;
    P2OUT = 0;
    P2DIR |= (BIT5 | BIT6);
    P2IES = 0;
    P2IFG = 0;
    P3OUT = 0;
    P3DIR = 0;
    P4OUT = 0;
    P4DIR = 0;
    P5OUT = 0;
    P5DIR = BIT0;
    P6OUT = 0;
    P6DIR = 0;

    TACTL = TACLR;
    TACCR0 = T_value;
    TACTL = TASSEL_1 | ID_0 | MC_1 | TAIE;

    __enable_interrupt();

    TM1637_init();
}


unsigned char inc_dec(signed char value, signed char value_max, signed char value_min)
{
    if(INC_button)
    {
        value++;

        if(value > value_max)
        {
            value = value_min;
        }
    }

    if(DEC_button)
    {
        value--;

        if(value < value_min)
        {
            value = value_max;
        }
    }

    delay_ms(40);

    return (value);
}
