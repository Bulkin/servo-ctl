#include "simplemesh.h"

#include <string.h>
#include <stdio.h>

/* Packet format:
  | START | SIZE | PAYLOAD | CRC |
     1       1     max 124   2 (BE)
*/

/* crc */

uint16_t calc_crc(uint8_t *data, uint8_t size)
{
	uint16_t crc = 0x1234;

	for (int i = 0; i < size; ++i) {
		uint8_t tmp = data[i] ^ (crc & 0xFF);
		tmp ^= tmp << 4;
		crc = (((tmp << 8) | ((crc >> 8) & 0xFF)) ^
		       (tmp >> 4) ^
		       (tmp << 3));
	}

	return crc;
}

int build_packet(uint8_t *packet_buf, uint8_t payload_size)
{
	int packet_size = 0;
	packet_buf[packet_size++] = SM_START_BYTE;
	packet_buf[packet_size++] = payload_size;

	packet_size += payload_size;

	uint16_t crc = calc_crc(packet_buf + 2, packet_size - 2);
	packet_buf[packet_size++] = crc & 0xFF;
	packet_buf[packet_size++] = crc >> 8;

	return packet_size;
}

int make_send_data_command(uint8_t *packet_buf, uint8_t p_id,
                           uint16_t addr, char* data, int data_size)
{
	int payload_size = 0;
	packet_buf[2 + payload_size++] = CMD_TX_DATA_REQUEST;
	packet_buf[2 + payload_size++] = addr & 0xFF;
	packet_buf[2 + payload_size++] = addr >> 8;
	packet_buf[2 + payload_size++] = 0;
	packet_buf[2 + payload_size++] = p_id;

	memcpy(packet_buf + 2 + payload_size,
	       data, data_size);
	payload_size += data_size;
	
	return build_packet(packet_buf, payload_size);
}

int make_uart_9600_command(uint8_t *packet_buf)
{
	int payload_size = 0;
	packet_buf[2 + payload_size++] = CMD_TX_SET_UART_MODE;
	packet_buf[2 + payload_size++] = DATA_BITS_8;
	packet_buf[2 + payload_size++] = PARITY_NONE;
	packet_buf[2 + payload_size++] = STOP_BITS_1;
	packet_buf[2 + payload_size++] = BAUD_9600;

	return build_packet(packet_buf, payload_size);
}

typedef enum {
	START,
	SIZE,
	PAYLOAD,
	CRC
} packet_parser_state;

extern void dispatch_cmd(uint8_t *cmd, int size);

int sm_parse_packet_stream(uint8_t c)
{
	static packet_parser_state state;
	static uint8_t packet_buf[SM_MAX_PACKET_SIZE];
	static int size;
	static int idx;
	uint16_t crc;
	
	switch (state) {

	case START:
		if (c == SM_START_BYTE) {
			idx = 0;
			state = SIZE;
		}
		break;

	case SIZE:
		size = c;
		if (size > (SM_MAX_PACKET_SIZE - 2))
			state = START;
		else
			state = PAYLOAD;
		break;

	case PAYLOAD:
		packet_buf[idx++] = c;
		if (idx >= (size + 2))
			state = CRC;
		else
			break;

	case CRC:
		crc = (packet_buf[size+1] << 8) | packet_buf[size];
		if (crc == calc_crc(packet_buf, size)) {
			packet_buf[size] = 0;
			dispatch_cmd(packet_buf, size);
		} 
		state = START;
	}

	return 0;
}
