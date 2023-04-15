#include "ToF10120.h"


void ToF10120_init(void)
{
    ToF10120_write(ToF10120_distance_measure_mode_reg, ToF10120_distance_filtered_measure_mode);
    ToF10120_write(ToF10120_distance_sending_mode_reg, ToF10120_distance_sending_through_UART_and_I2C);
    ToF10120_set_distance_deviation(0x00);
}


void ToF10120_write(unsigned char address, unsigned char data_byte)
{
    i2c_usi_mst_gen_start();
    i2c_usi_mst_send_address(ToF10120_I2C_address, wr);
    i2c_usi_mst_send_byte(address);
    delay_us(40);
    i2c_usi_mst_send_byte(data_byte);
    i2c_usi_mst_gen_stop();
}


unsigned char ToF10120_read_byte(unsigned char address)
{
    unsigned char retval = 0x00;
    
    i2c_usi_mst_gen_start();
    i2c_usi_mst_send_address(ToF10120_I2C_address, wr);
    i2c_usi_mst_send_byte(address);
    i2c_usi_mst_gen_stop();
    delay_us(40);

    i2c_usi_mst_gen_repeated_start();
    i2c_usi_mst_send_address(ToF10120_I2C_address, rd);
    retval = i2c_usi_mst_read_byte();
    i2c_usi_mst_send_n_ack(0);
    i2c_usi_mst_gen_stop();

    return retval;
}


unsigned int ToF10120_read_word(unsigned char address)
{   
    unsigned char LB = 0x00;
    unsigned char HB = 0x00;
    unsigned int retval = 0x0000;

    i2c_usi_mst_gen_start();
    i2c_usi_mst_send_address(ToF10120_I2C_address, wr);
    i2c_usi_mst_send_byte(address);
    i2c_usi_mst_gen_stop();
    delay_us(40);

    i2c_usi_mst_gen_repeated_start();
    i2c_usi_mst_send_address(ToF10120_I2C_address, rd);

    HB = i2c_usi_mst_read_byte();
    i2c_usi_mst_send_n_ack(1);
    LB = i2c_usi_mst_read_byte();
    i2c_usi_mst_send_n_ack(0);

    i2c_usi_mst_gen_stop();

    retval = (unsigned int)HB;
    retval <<= 0X08;
    retval |= (unsigned int)LB;

    return retval;
}


void ToF10120_set_distance_deviation(signed int value)
{
    unsigned char LB = 0x00;
    unsigned char HB = 0x00;

    LB = ((unsigned char)(value & 0x00FF));
    HB = ((unsigned char)((value & 0xFF00) >> 0x08));

    ToF10120_write(HB, ToF10120_distance_deviation_reg);
    ToF10120_write(LB, (ToF10120_distance_deviation_reg + 1));
}


void ToF10120_set_I2C_address(unsigned char value)
{
    ToF10120_write(ToF10120_I2C_slave_address_reg, value);
}


unsigned int ToF10120_get_distance(void)
{
    return (ToF10120_read_word(ToF10120_distance_reg));
}


unsigned int ToF10120_get_filtered_distance(void)
{
    return (ToF10120_read_word(ToF10120_filter_distance_reg));
}


unsigned int ToF10120_distance_deviation(void)
{
    return (ToF10120_read_word(ToF10120_distance_deviation_reg));
}


unsigned int ToF10120_get_max_distance(void)
{
    return (ToF10120_read_word(ToF10120_max_measuring_distance_reg));
}


unsigned char ToF10120_get_I2C_address(void)
{
    return (ToF10120_read_byte(ToF10120_I2C_slave_address_reg));
}


unsigned char ToF10120_get_distance_measure_mode(void)
{
    return (ToF10120_read_byte(ToF10120_distance_measure_mode_reg));
}


unsigned char ToF10120_get_distance_sending_mode(void)
{
    return (ToF10120_read_byte(ToF10120_distance_sending_mode_reg));
}
