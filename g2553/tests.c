#include <stdio.h>
#include <stdint.h>

#include "simplemesh.h"

void print_data(char* data, int size)
{
	printf("DATA: ");
	for (int i = 0; i < size; ++i) {
		printf("%X ", (unsigned char)data[i]);
	}
	printf("\n");
}

void test_simplemesh()
{
	char packet_buf[SM_MAX_PACKET_SIZE];
	char data1[] = "test";
	int data1_size = sizeof(data1) - 1;
	
	int size = make_send_data_command(packet_buf, 14, 04,  
	                                  data1, data1_size);
	print_data(packet_buf, size);

	size = make_uart_9600_command(packet_buf);
	print_data(packet_buf, size);

	uint8_t data2[] = { 171, 10, 34, 0, 0, 0, 255, 
	                    199, 84, 69, 83, 84, 96, 173 };
	int data2_size = sizeof(data2);
	for (int i = 0; i < data2_size; ++i)
		sm_parse_packet_stream(data2[i]);
}

void dispatch_cmd(uint8_t *cmd, int size)
{
	uint16_t source;

	switch (cmd[0]) {
	case CMD_RX_DATA_INDICATION:
		source = cmd[1] | cmd[2] << 8;
		printf("from: %X  ", source);
		print_data(cmd + 6, size - 6);
		break;
	}
}

int main()
{
	test_simplemesh();
	return 0;
}
