#ifndef SIMPLEMESH_H
#define SIMPLEMESH_H

#include <stdint.h>

/* Construct packet inside packet_buf
   Actual payload should start at offset 2 
   Returns resulting packet size */
int build_packet(uint8_t *packet_buf, uint8_t payload_size);

int make_send_data_command(uint8_t *packet_buf, uint8_t p_id,
                           uint16_t addr, char* data, int data_size);

int make_uart_9600_command(uint8_t *packet_buf);

/* Main loop that parses a packet stream defined by the next_char function
   NON-REENTRANT */
int sm_parse_packet_stream(uint8_t c);

typedef char (*getchar_func)();

/* protocol constants */

#define SM_MAX_PACKET_SIZE 127

#define SM_START_BYTE                         0xab

#define CMD_TX_TEST_REQUEST                0x01
#define CMD_TX_RESET_REQUEST               0x03
#define CMD_TX_SETTINGS_REQUEST            0x04
#define CMD_TX_SET_UART_MODE               0x05
#define CMD_TX_SLEEP_REQUEST               0x06
#define CMD_TX_DATA_REQUEST                0x20
#define CMD_TX_SET_ADDRESS_REQUEST         0x23
#define CMD_TX_GET_ADDRESS_REQUEST         0x24
#define CMD_TX_SET_PANID_REQUEST           0x26
#define CMD_TX_GET_PANID_REQUEST           0x27
#define CMD_TX_SET_CHANNEL_REQUEST         0x29
#define CMD_TX_GET_CHANNEL_REQUEST         0x2a
#define CMD_TX_SET_RECEIVER_STATE_REQUEST  0x2c
#define CMD_TX_GET_RECEIVER_STATE_REQUEST  0x2d
#define CMD_TX_SET_TRANSMIT_POWER_REQUEST  0x2f
#define CMD_TX_GET_TRANSMIT_POWER_REQUEST  0x30
#define CMD_TX_SET_SECURITY_KEY_REQUEST    0x32
#define CMD_TX_SET_ACK_STATE_REQUEST       0x35
#define CMD_TX_GET_ACK_STATE_REQUEST       0x36
#define CMD_TX_SET_LED_STATE_REQUEST       0x80

#define CMD_RX_ACK                         0x00
#define CMD_RX_TEST_RESPONSE               0x02
#define CMD_RX_WAKEUP_INDICATION           0x07
#define CMD_RX_DATA_CONFIRMATION           0x21
#define CMD_RX_DATA_INDICATION             0x22
#define CMD_RX_GET_ADDRESS_RESPONSE        0x25
#define CMD_RX_GET_PANID_RESPONSE          0x28
#define CMD_RX_GET_CHANNEL_RESPONSE        0x2b
#define CMD_RX_GET_RECEIVER_STATE_RESPONSE 0x2e
#define CMD_RX_GET_TRANSMIT_POWER_RESPONSE 0x31
#define CMD_RX_SET_ACK_STATE_RESPONSE      0x37

#define APP_UART_START_BYTE        		   0xab
#define SAVE_CURRENT_SETTINGS			   0x10
#define RESTORE_CURRENT_SETTINGS		   0x15
#define DATA_BITS_5						   0x00
#define DATA_BITS_6						   0x01
#define DATA_BITS_7						   0x02
#define DATA_BITS_8						   0x03
#define PARITY_NONE						   0x00
#define PARITY_ODD						   0x01
#define PARITY_EVEN						   0x02
#define PARITY_FORCE_1					   0x03
#define PARITY_FORCE_2					   0x04
#define STOP_BITS_1						   0x00
#define STOP_BITS_2						   0x01 //(1.5 for 5 data bits)
#define BAUD_RESERVED					   0x00
#define BAUD_50							   0x01
#define BAUD_75							   0x02
#define BAUD_110						   0x03 
#define BAUD_150						   0x04 
#define BAUD_300						   0x05 
#define BAUD_1200						   0x06 
#define BAUD_2400						   0x07 
#define BAUD_4800						   0x08 
#define BAUD_9600						   0x09 
#define BAUD_19200						   0x0a 
#define BAUD_38400						   0x0b 
#define BAUD_57600						   0x0c 
#define BAUD_115200						   0x0d 
#define BAUD_230400						   0x0e 
#define BAUD_460800						   0x0f 
#define BAUD_2000  						   0x10 
#define BAUD_4000  						   0x11 
#define BAUD_8000  						   0x12 
#define BAUD_10000 						   0x13 
#define BAUD_20000 						   0x14 
#define BAUD_30000 						   0x15 
#define BAUD_40000 						   0x16 
#define BAUD_50000 						   0x17 
#define BAUD_60000 						   0x18 
#define BAUD_70000 						   0x19 
#define BAUD_80000 						   0x1a 
#define BAUD_90000 						   0x1b 
#define BAUD_100000						   0x1c 
#define BAUD_200000						   0x1d 
#define BAUD_300000						   0x1e 
#define BAUD_400000						   0x1f 
#define DATA_OPTION_NONE				   0x00
#define DATA_OPTION_ACK					   0x01
#define DATA_OPTION_SECURITY			   0x02
#define CHANNEL_11						   0x0B
#define CHANNEL_12						   0x0C
#define CHANNEL_13						   0x0D
#define CHANNEL_14						   0x0E
#define CHANNEL_15						   0x0F
#define CHANNEL_16						   0x10
#define CHANNEL_17						   0x11
#define CHANNEL_18						   0x12
#define CHANNEL_19						   0x13
#define CHANNEL_20						   0x14
#define CHANNEL_21						   0x15
#define CHANNEL_22						   0x16
#define CHANNEL_23						   0x17
#define CHANNEL_24						   0x18
#define CHANNEL_25						   0x19
#define CHANNEL_26						   0x1a	// Not certified...
#define RX_ON							   0x01
#define TX_ON							   0x00
#define TX_POWER_3_0_DBM   	 			   0x00
#define TX_POWER_2_8_DBM   	 			   0x01
#define TX_POWER_2_3_DBM   	 			   0x02
#define TX_POWER_1_8_DBM   	 			   0x03
#define TX_POWER_1_3_DBM   	 			   0x04
#define TX_POWER_0_7_DBM   	 			   0x05
#define TX_POWER_0_DBM     	 			   0x06
#define TX_POWER_M_1_DBM   	 			   0x07
#define TX_POWER_M_2_DBM   	 			   0x08
#define TX_POWER_M_3_DBM   	 			   0x09
#define TX_POWER_M_4_DBM   	 			   0x0a
#define TX_POWER_M_5_DBM   	 			   0x0b
#define TX_POWER_M_7_DBM   	 			   0x0c
#define TX_POWER_M_9_DBM   	 			   0x0d
#define TX_POWER_M_12_DBM  	 			   0x0e
#define TX_POWER_M_17_DBM  	 			   0x0f
#define ACK_STATE_DISABLE				   0x00
#define ACK_STATE_ENABLE				   0x01
#define LED_OFF							   0x00
#define LED_ON							   0x01
#define LED_TOGGLE						   0x02

#define RX_STATE_START					   0x00
#define RX_STATE_LENGTH					   0x01
#define RX_STATE_COMMAND_ID				   0x02
#define RX_STATE_PAYLOAD				   0x03
#define RX_STATE_FCS					   0x04

#define COMMAND_ID_START				   0xff


#endif
