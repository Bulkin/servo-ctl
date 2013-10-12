#include <stdio.h>
#include <stdint.h>

#include "simplemesh.h"

void print_data(char* data, int size)
{
	printf("DATA: ");
	for (int i = 0; i < size; ++i) {
		printf("%x ", (unsigned char)data[i]);
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
}

int main()
{
	test_simplemesh();
	return 0;
}
