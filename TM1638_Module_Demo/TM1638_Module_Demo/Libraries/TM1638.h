#include "MSP430.h"


#define HIGH											 1
#define LOW												 0

#define DIO_pin                                          BIT0
#define CLK_pin                                          BIT1
#define STB_pin										 	 BIT2

#define TM1638_IO_DIR          							 P2DIR
#define TM1638_IO_OUT          					 		 P2OUT
#define TM1638_IO_IN           							 P2IN

#define STB_DIR_OUT()									 do{TM1638_IO_DIR |= STB_pin;}while(0)
#define CLK_DIR_OUT()									 do{TM1638_IO_DIR |= CLK_pin;}while(0)
#define DIO_DIR_OUT()									 do{TM1638_IO_DIR |= DIO_pin;}while(0)

#define STB_HIGH()										 do{TM1638_IO_OUT |= STB_pin;}while(0)
#define STB_LOW()										 do{TM1638_IO_OUT &= ~STB_pin;}while(0)
#define CLK_HIGH()										 do{TM1638_IO_OUT |= CLK_pin;}while(0)
#define CLK_LOW()										 do{TM1638_IO_OUT &= ~CLK_pin;}while(0)
#define DIO_HIGH()										 do{TM1638_IO_OUT |= DIO_pin;}while(0)
#define DIO_LOW()										 do{TM1638_IO_OUT &= ~DIO_pin;}while(0)

#define DIO_DIR_IN()                                     do{TM1638_IO_DIR &= ~DIO_pin;}while(0)

#define DIO_IN()										 (TM1638_IO_IN & DIO_pin)

#define number_of_LEDs									 16

#define	data_instruction_set							 0x40
#define	display_control_instruction_set					 0x80
#define	address_instruction_set							 0xC0

#define write_register									 (data_instruction_set | 0x00)
#define read_register									 (data_instruction_set | 0x02)
#define auto_address                                     (data_instruction_set | 0x00)
#define fixed_address                                    (data_instruction_set | 0x04)
#define normal_mode                                      (data_instruction_set | 0x00)
#define test_mode                                        (data_instruction_set | 0x08)

#define display_off                                      (display_control_instruction_set | 0x00)
#define brightness_5_pc                                  (display_control_instruction_set | 0x08)
#define brightness_10_pc                                 (display_control_instruction_set | 0x09)
#define brightness_25_pc                                 (display_control_instruction_set | 0x0A)
#define brightness_60_pc                                 (display_control_instruction_set | 0x0B)
#define brightness_70_pc                                 (display_control_instruction_set | 0x0C)
#define brightness_75_pc                                 (display_control_instruction_set | 0x0D)
#define brightness_80_pc                                 (display_control_instruction_set | 0x0E)
#define brightness_90_pc                                 (display_control_instruction_set | 0x0F)
#define display_on                                       (display_control_instruction_set | 0x0F)


void TM1638_init(unsigned char brightness);
void TM1638_write(unsigned char	value);
unsigned char TM1638_read(void);
void TM1638_send_command(unsigned char value);
unsigned char TM1638_read_key(void);
void TM1638_send_data(unsigned char address, unsigned char value);
void TM1638_set_all_LEDs(unsigned char value);
void TM1638_clear_display(void);
