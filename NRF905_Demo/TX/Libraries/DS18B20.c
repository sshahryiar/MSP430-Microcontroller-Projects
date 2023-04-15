#include "DS18B20.h"


void DS18B20_init(void)                             
{                                       
	onewire_reset();
	delay_ms(100);
}              


signed int DS18B20_get_raw_temperature(void)
{                                               
	unsigned char msb = 0x00;
	unsigned char lsb = 0x00;
	signed int temp = 0x0000;
	                       
	onewire_reset();     
	onewire_write(skip_ROM);        
	onewire_write(convert_T); 
   	
	switch(resolution)   
	{                                                  
		case 12:
		{	                                         
			delay_ms(750);
			break;
		}                
		case 11:                                     
		{              
			delay_ms(375);
			break;
		}           
		case 10:                             
		{                                 
			delay_ms(188);   
			break;
		}                                        
		case 9:                                   
		{                                                
			delay_ms(94);                 
			break;                            
		}                        
	}                  

	onewire_reset(); 

	onewire_write(skip_ROM);                 
	onewire_write(read_scratchpad);

	lsb = onewire_read();
	msb = onewire_read();

	temp = msb;                           
	temp <<= 8;
	temp |= lsb;
   	
	delay_ms(40);   	
	
	return temp;
}


float DS18B20_get_temperature(signed int temp)
{
    float value = 0.0;

    switch(resolution)
    {
        case 12:
        {
            value = ((float)temp * 0.0625);
            break;
        }
        case 11:
        {
            value = ((float)temp * 0.125);
            break;
        }
        case 10:
        {
            value = ((float)temp * 0.25);
            break;
        }
        case 9:
        {
            value = ((float)temp * 0.5);
            break;
        }
    }

    return value;
}

