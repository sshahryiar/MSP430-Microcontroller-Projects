#include "NRF905.h"


void NRF905_set_DIOs(void)
{
    NRF905_TXEN_PIN_DIR_OUT;
    NRF905_TRX_CE_PIN_DIR_OUT;
    NRF905_PWR_PIN_DIR_OUT;
    NRF905_MOSI_PIN_DIR_OUT;
    NRF905_SCK_PIN_DIR_OUT;
    NRF905_CSN_PIN_DIR_OUT;
    NRF905_AM_PIN_DIR_OUT;
    NRF905_CD_PIN_DIR_OUT;
    NRF905_DR_PIN_DIR_IN;
    NRF905_MISO_PIN_DIR_IN;
    NRF905_UCLK_PIN_DIR_IN;

    P3OUT = 0x00;
    P2OUT = 0x00;

    NRF905_CSN_PIN_HIGH;
    NRF905_SCK_PIN_LOW;
    NRF905_PWR_PIN_HIGH;
    NRF905_TRX_CE_PIN_LOW;
    NRF905_TXEN_PIN_LOW;
}


void NRF905_init(void)
{
    unsigned char s = 0x00;

    const unsigned char NRF905_RF_config[NRF905_RF_config_buffer_length] =
    {
         NRF905_WC_command,
         NRF905_RF_config_BYTE_0,
         NRF905_RF_config_BYTE_1,
         NRF905_RF_config_BYTE_2,
         NRF905_RF_config_BYTE_3,
         NRF905_RF_config_BYTE_4,
         NRF905_RF_config_BYTE_5,
         NRF905_RF_config_BYTE_6,
         NRF905_RF_config_BYTE_7,
         NRF905_RF_config_BYTE_8,
         NRF905_RF_config_BYTE_9
    };

    NRF905_set_DIOs();

    NRF905_CSN_PIN_LOW;

    for(s = 0x00; s < NRF905_RF_config_buffer_length; s++)
    {
        NRF905_SPI_write(NRF905_RF_config[s]);
    }

    NRF905_CSN_PIN_HIGH;
}


unsigned char NRF905_SPI_read(void)
{
    unsigned char s = 0x08;
    unsigned char value = 0x00;

    while(s > 0)
    {
        value <<= 1;

        NRF905_SCK_PIN_HIGH;

        if(NRF905_MISO_PIN_IN)
        {
            value |= 0x01;
        }

        NRF905_SCK_PIN_LOW;

        s--;
     };

     return value;
}


void NRF905_SPI_write(unsigned char value)
{
    unsigned char s = 0x08;

    while(s > 0)
    {
        if(((value & 0x80) != 0))
        {
            NRF905_MOSI_PIN_HIGH;
        }
        else
        {
            NRF905_MOSI_PIN_LOW;
        }

        NRF905_SCK_PIN_HIGH;

        value <<= 0x01;

        NRF905_SCK_PIN_LOW;

        s--;
    };
}


void NRF905_set_TX_mode(void)
{
    NRF905_TXEN_PIN_HIGH;
    NRF905_TRX_CE_PIN_HIGH;
    delay_ms(40);
}


void NRF905_set_RX_mode(void)
{
    NRF905_TXEN_PIN_LOW;
    NRF905_TRX_CE_PIN_HIGH;
    delay_ms(40);
}


unsigned char NRF905_check_DR(void)
{
    if(NRF905_DR_PIN_IN != 0x00)
    {
        return 0x01;
    }
    else
    {
        return 0x00;
    }
}


void NRF905_TX_packet(void)
{
    unsigned char s = 0x00;
    const unsigned char NRF905_TX_address[NRF905_TX_address_length] =
    {
         NRF905_TX_address_0,
         NRF905_TX_address_1,
         NRF905_TX_address_2,
         NRF905_TX_address_3
    };

    NRF905_CSN_PIN_LOW;
    NRF905_SPI_write(NRF905_WTP_command);

    for(s = 0x00; s < NRF905_TX_buffer_length; s++)
    {
        NRF905_SPI_write(NRF905_TX_buffer[s]);
    }

    NRF905_CSN_PIN_HIGH;
    delay_ms(1);

    NRF905_CSN_PIN_LOW;
    NRF905_SPI_write(NRF905_WTA_command);

    for (s = 0x00; s < NRF905_TX_address_length; s++)
    {
        NRF905_SPI_write(NRF905_TX_address[s]);
    }

    NRF905_CSN_PIN_HIGH;

    NRF905_TRX_CE_PIN_HIGH;
    delay_ms(40);
    NRF905_TRX_CE_PIN_LOW;
}


void NRF905_RX_packet(void)
{
    unsigned char s = 0x00;

    delay_ms(100);

    NRF905_TRX_CE_PIN_LOW;
    NRF905_CSN_PIN_LOW;
    delay_ms(1);

    NRF905_SPI_write(NRF905_RRP_command);

    for(s = 0x00; s < NRF905_RX_buffer_length; s++)
    {
        NRF905_RX_buffer[s] = NRF905_SPI_read();
    }

    NRF905_CSN_PIN_HIGH;
    delay_ms(40);

    NRF905_TRX_CE_PIN_HIGH;
}


void NRF905_TX(void)
{
    NRF905_set_TX_mode();
    NRF905_TX_packet();
}


void NRF905_RX(void)
{
    NRF905_set_RX_mode();
    while(NRF905_check_DR() == 0x00);
    delay_ms(40);
    NRF905_RX_packet();
    delay_ms(40);
}
