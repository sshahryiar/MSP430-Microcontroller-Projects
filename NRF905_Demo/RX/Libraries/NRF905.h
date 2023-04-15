#include <msp430.h>
#include <delay.h>


#define NRF905_TXEN_PIN_DIR                     P3DIR
#define NRF905_TRX_CE_PIN_DIR                   P3DIR
#define NRF905_PWR_PIN_DIR                      P3DIR
#define NRF905_UCLK_PIN_DIR                     P2DIR
#define NRF905_MISO_PIN_DIR                     P2DIR
#define NRF905_MOSI_PIN_DIR                     P2DIR
#define NRF905_SCK_PIN_DIR                      P2DIR
#define NRF905_CSN_PIN_DIR                      P2DIR
#define NRF905_AM_PIN_DIR                       P2DIR
#define NRF905_DR_PIN_DIR                       P2DIR
#define NRF905_CD_PIN_DIR                       P2DIR

#define NRF905_TXEN_PIN_OUT_PORT                P3OUT
#define NRF905_TRX_CE_PIN_OUT_PORT              P3OUT
#define NRF905_PWR_PIN_OUT_PORT                 P3OUT
#define NRF905_UCLK_PIN_OUT_PORT                P2OUT
#define NRF905_MOSI_PIN_OUT_PORT                P2OUT
#define NRF905_SCK_PIN_OUT_PORT                 P2OUT
#define NRF905_CSN_PIN_OUT_PORT                 P2OUT
#define NRF905_AM_PIN_OUT_PORT                  P2OUT
#define NRF905_CD_PIN_OUT_PORT                  P2OUT

#define NRF905_MISO_PIN_IN_PORT                 P2IN
#define NRF905_DR_PIN_IN_PORT                   P2IN

#define NRF905_TXEN_PIN                         BIT2
#define NRF905_TRX_CE_PIN                       BIT1
#define NRF905_PWR_PIN                          BIT0
#define NRF905_UCLK_PIN                         BIT7
#define NRF905_MISO_PIN                         BIT3
#define NRF905_MOSI_PIN                         BIT2
#define NRF905_SCK_PIN                          BIT1
#define NRF905_CSN_PIN                          BIT0
#define NRF905_AM_PIN                           BIT5
#define NRF905_DR_PIN                           BIT4
#define NRF905_CD_PIN                           BIT6


#define NRF905_TXEN_PIN_DIR_OUT                 NRF905_TXEN_PIN_DIR |= NRF905_TXEN_PIN
#define NRF905_TRX_CE_PIN_DIR_OUT               NRF905_TRX_CE_PIN_DIR |= NRF905_TRX_CE_PIN
#define NRF905_PWR_PIN_DIR_OUT                  NRF905_PWR_PIN_DIR |= NRF905_PWR_PIN
#define NRF905_MOSI_PIN_DIR_OUT                 NRF905_MOSI_PIN_DIR |= NRF905_MOSI_PIN
#define NRF905_SCK_PIN_DIR_OUT                  NRF905_SCK_PIN_DIR |= NRF905_SCK_PIN
#define NRF905_CSN_PIN_DIR_OUT                  NRF905_CSN_PIN_DIR |= NRF905_CSN_PIN
#define NRF905_AM_PIN_DIR_OUT                   NRF905_AM_PIN_DIR |= NRF905_AM_PIN
#define NRF905_CD_PIN_DIR_OUT                   NRF905_CD_PIN_DIR |= NRF905_CD_PIN
#define NRF905_UCLK_PIN_DIR_IN                  NRF905_UCLK_PIN_DIR &= ~NRF905_UCLK_PIN
#define NRF905_DR_PIN_DIR_IN                    NRF905_DR_PIN_DIR &= ~NRF905_DR_PIN
#define NRF905_MISO_PIN_DIR_IN                  NRF905_MISO_PIN_DIR &= ~NRF905_MISO_PIN


#define NRF905_TXEN_PIN_HIGH                    NRF905_TXEN_PIN_OUT_PORT |= NRF905_TXEN_PIN
#define NRF905_TXEN_PIN_LOW                     NRF905_TXEN_PIN_OUT_PORT &= ~NRF905_TXEN_PIN

#define NRF905_TRX_CE_PIN_HIGH                  NRF905_TRX_CE_PIN_OUT_PORT |= NRF905_TRX_CE_PIN
#define NRF905_TRX_CE_PIN_LOW                   NRF905_TRX_CE_PIN_OUT_PORT &= ~NRF905_TRX_CE_PIN

#define NRF905_PWR_PIN_HIGH                     NRF905_PWR_PIN_OUT_PORT |= NRF905_PWR_PIN
#define NRF905_PWR_PIN_LOW                      NRF905_PWR_PIN_OUT_PORT &= ~NRF905_PWR_PIN

#define NRF905_MOSI_PIN_HIGH                    NRF905_MOSI_PIN_OUT_PORT |= NRF905_MOSI_PIN
#define NRF905_MOSI_PIN_LOW                     NRF905_MOSI_PIN_OUT_PORT &= ~NRF905_MOSI_PIN

#define NRF905_SCK_PIN_HIGH                     NRF905_SCK_PIN_OUT_PORT |= NRF905_SCK_PIN
#define NRF905_SCK_PIN_LOW                      NRF905_SCK_PIN_OUT_PORT &= ~NRF905_SCK_PIN

#define NRF905_CSN_PIN_HIGH                     NRF905_CSN_PIN_OUT_PORT |= NRF905_CSN_PIN
#define NRF905_CSN_PIN_LOW                      NRF905_CSN_PIN_OUT_PORT &= ~NRF905_CSN_PIN

#define NRF905_AM_PIN_HIGH                      NRF905_AM_PIN_OUT_PORT |= NRF905_AM_PIN
#define NRF905_AM_PIN_LOW                       NRF905_AM_PIN_OUT_PORT &= ~NRF905_AM_PIN

#define NRF905_CD_PIN_HIGH                      NRF905_CD_PIN_OUT_PORT |= NRF905_CD_PIN
#define NRF905_CD_PIN_LOW                       NRF905_CD_PIN_OUT_PORT &= ~NRF905_CD_PIN

#define NRF905_MISO_PIN_IN                      NRF905_MISO_PIN_IN_PORT & NRF905_MISO_PIN
#define NRF905_DR_PIN_IN                        NRF905_DR_PIN_IN_PORT & NRF905_DR_PIN


#define NRF905_WC_command                       0x00
#define NRF905_RC_command                       0x10
#define NRF905_WTP_command                      0x20
#define NRF905_RTP_command                      0x21
#define NRF905_WTA_command                      0x22
#define NRF905_RTA_command                      0x23
#define NRF905_RRP_command                      0x24


#define NRF905_CHANNEL                          66

#define NRF905_HFREQ_PLL_433MHz               0x00
#define NRF905_HFREQ_PLL_868MHz_915MHz        0x02

#define NRF905_PA_PWR_neg_10dBm               0x00
#define NRF905_PA_PWR_neg_2dBm                0x04
#define NRF905_PA_PWR_6dBm                    0x08
#define NRF905_PA_PWR_10dBm                   0x0C

#define NRF905_RX_RED_PWR_NORMAL              0x00
#define NRF905_RX_RED_PWR_REDUCED             0x10

#define NRF905_AUTO_RETRAN_NO                 0x00
#define NRF905_AUTO_RETRAN_YES                0x20

#define NRF905_RX_AFW_1_byte                  0x00
#define NRF905_RX_AFW_4_byte                  0x04

#define NRF905_TX_AFW_1_byte                  0x00
#define NRF905_TX_AFW_4_byte                  0x40

#define NRF905_RX_PW_1_BYTE                   0x01
#define NRF905_RX_PW_2_BYTE                   0x02
#define NRF905_RX_PW_4_BYTE                   0x04
#define NRF905_RX_PW_8_BYTE                   0x08
#define NRF905_RX_PW_16_BYTE                  0x10
#define NRF905_RX_PW_32_BYTE                  0x20

#define NRF905_TX_PW_1_BYTE                   0x01
#define NRF905_TX_PW_2_BYTE                   0x02
#define NRF905_TX_PW_4_BYTE                   0x04
#define NRF905_TX_PW_8_BYTE                   0x08
#define NRF905_TX_PW_16_BYTE                  0x10
#define NRF905_TX_PW_32_BYTE                  0x20

#define NRF905_RX_ADDRESS_BYTE_0              0xCC
#define NRF905_RX_ADDRESS_BYTE_1              0xCC
#define NRF905_RX_ADDRESS_BYTE_2              0xCC
#define NRF905_RX_ADDRESS_BYTE_3              0xCC

#define NRF905_UP_CLK_FREQ_4MHz               0x00
#define NRF905_UP_CLK_FREQ_2MHz               0x01
#define NRF905_UP_CLK_FREQ_1MHz               0x02
#define NRF905_UP_CLK_FREQ_500kHz             0x03

#define NRF905_UP_CLK_EN_NO                   0x00
#define NRF905_UP_CLK_EN_YES                  0x04

#define NRF905_XOF_4MHz                       0x00
#define NRF905_XOF_8MHz                       0x08
#define NRF905_XOF_12MHz                      0x10
#define NRF905_XOF_16MHz                      0x18
#define NRF905_XOF_20MHz                      0x20

#define NRF905_CRC_EN_NO                      0x00
#define NRF905_CRC_EN_YES                     0x40

#define NRF905_CRC_MODE_8_BIT                 0x00
#define NRF905_CRC_MODE_16_BIT                0x80


#define NRF905_RF_config_BYTE_0              (NRF905_CHANNEL & 0x00FF)
#define NRF905_RF_config_BYTE_1              (NRF905_AUTO_RETRAN_YES + NRF905_RX_RED_PWR_NORMAL + NRF905_PA_PWR_10dBm + NRF905_HFREQ_PLL_433MHz + ((NRF905_CHANNEL & 0x0100) >> 8))
#define NRF905_RF_config_BYTE_2              (NRF905_TX_AFW_4_byte + NRF905_RX_AFW_4_byte)
#define NRF905_RF_config_BYTE_3              (NRF905_RX_PW_32_BYTE & 0x3F)
#define NRF905_RF_config_BYTE_4              (NRF905_TX_PW_32_BYTE & 0x3F)
#define NRF905_RF_config_BYTE_5              NRF905_RX_ADDRESS_BYTE_0
#define NRF905_RF_config_BYTE_6              NRF905_RX_ADDRESS_BYTE_1
#define NRF905_RF_config_BYTE_7              NRF905_RX_ADDRESS_BYTE_2
#define NRF905_RF_config_BYTE_8              NRF905_RX_ADDRESS_BYTE_3
#define NRF905_RF_config_BYTE_9              (NRF905_CRC_MODE_16_BIT + NRF905_CRC_EN_YES + NRF905_XOF_16MHz + NRF905_UP_CLK_EN_NO + NRF905_UP_CLK_FREQ_4MHz)

#define NRF905_TX_address_0                  0xCC
#define NRF905_TX_address_1                  0xCC
#define NRF905_TX_address_2                  0xCC
#define NRF905_TX_address_3                  0xCC

#define NRF905_TX_buffer_length              32
#define NRF905_RX_buffer_length              32

#define NRF905_TX_address_length              4

#define NRF905_RF_config_buffer_length       11


unsigned char NRF905_TX_buffer[NRF905_TX_buffer_length];
unsigned char NRF905_RX_buffer[NRF905_RX_buffer_length];


void NRF905_set_DIOs(void);
void NRF905_init(void);
unsigned char NRF905_SPI_read(void);
void NRF905_SPI_write(unsigned char value);
void NRF905_set_TX_mode(void);
void NRF905_set_RX_mode(void);
unsigned char NRF905_check_DR(void);
void NRF905_TX_packet(void);
void NRF905_RX_packet(void);
void NRF905_TX(void);
void NRF905_RX(void);
