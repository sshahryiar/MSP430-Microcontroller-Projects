#include "TM1638.h"


void TM1638_init(unsigned char brightness)
{
	STB_DIR_OUT();
	CLK_DIR_OUT();
	DIO_DIR_OUT();
	DIO_LOW();
	CLK_LOW();
	STB_HIGH();

	TM1638_send_command(brightness);       
	TM1638_send_command(normal_mode);
}


void TM1638_write(unsigned char	value)
{
	unsigned char i = 0x08;
    
    DIO_DIR_OUT();

    while(i > 0x00)
    {
    	CLK_LOW();

    	if((value & 0x01) == HIGH)
    	{
    		DIO_HIGH();	
    	}
    	else
    	{
    		DIO_LOW();
    	}

    	value >>= 0x01;

    	CLK_HIGH();

    	i--;
    }
}


unsigned char TM1638_read(void)
{
	unsigned char i = 0x08;
	unsigned char value = 0x00;

    DIO_DIR_IN();

    while(i > 0x00)
    {
    	value >>= 0x01;

    	CLK_LOW();

    	if(DIO_IN() != LOW)
    	{
    		value |= 0x80;
    	}

    	CLK_HIGH();

    	i--;
	}

	return value;
}


void TM1638_send_command(unsigned char value)
{
	STB_LOW();
	TM1638_write(value);
    STB_HIGH();
}


unsigned char TM1638_read_key(void)
{
	unsigned char i = 0x00;
	unsigned char key_value = 0x00;

	unsigned char temp[0x04] = {0x00, 0x00, 0x00, 0x00};
  
    STB_LOW();
	TM1638_write(read_register);

	for(i = 0x00; i < 0x04; i++)		
  	{
        temp[i] = TM1638_read();
    }

    STB_HIGH();		

	for(i = 0x00; i < 0x04; i++)
	{
        key_value |= (temp[i] << i);
    }
    	
    for(i = 0x00; i < 0x08; i++)
    {
        if((0x01 << i) == key_value)
        {
        	break;
        }
    }

	return i;
}


void TM1638_send_data(unsigned char address, unsigned char value)
{
	TM1638_send_command(fixed_address);

    STB_LOW();
	TM1638_write(address_instruction_set | address);
	TM1638_write(value);
    STB_HIGH();
}


void TM1638_set_all_LEDs(unsigned char value)
{
	unsigned char i = 0x00;

	for(i = 0x00; i < number_of_LEDs; i++)	
	{   
		TM1638_send_data(i, value);
	}
}


void TM1638_clear_display(void)
{
	TM1638_set_all_LEDs(0x00);
}
