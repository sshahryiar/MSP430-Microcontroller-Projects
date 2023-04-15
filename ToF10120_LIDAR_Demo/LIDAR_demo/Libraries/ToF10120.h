#include "I2C.h"
#include "delay.h"


#define ToF10120_I2C_address                                0x52

#define ToF10120_distance_reg                               0x00
#define ToF10120_filter_distance_reg                        0x04
#define ToF10120_distance_deviation_reg                     0x06
#define ToF10120_distance_measure_mode_reg                  0x08
#define ToF10120_distance_sending_mode_reg                  0x09
#define ToF10120_max_measuring_distance_reg                 0x0C
#define ToF10120_I2C_slave_address_reg                      0x0F

#define ToF10120_distance_filtered_measure_mode             0x00
#define ToF10120_distance_real_time_measure_mode            0x01

#define ToF10120_distance_sending_through_UART              0x00
#define ToF10120_distance_sending_through_UART_and_I2C      0x01


void ToF10120_init(void);
void ToF10120_write(unsigned char address, unsigned char data_byte);
unsigned char ToF10120_read_byte(unsigned char address);
unsigned int ToF10120_read_word(unsigned char address);
void ToF10120_set_distance_deviation(signed int value);
void ToF10120_set_I2C_address(unsigned char value);
unsigned int ToF10120_get_distance(void);
unsigned int ToF10120_get_filtered_distance(void);
unsigned int ToF10120_distance_deviation(void);
unsigned int ToF10120_get_max_distance(void);
unsigned char ToF10120_get_distance_measure_mode(void);
unsigned char ToF10120_get_distance_sending_mode(void);
