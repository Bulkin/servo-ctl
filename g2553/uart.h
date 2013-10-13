#ifndef UART_H
#define UART_H

typedef enum {
	UART_BR_9600,
	UART_BR_115200
} baudrate;

void init_uart();
char uart_getchar();
int uart_read_line(char* dest, int max);
int uart_send_data(char* data, int data_size);
int uart_send_char(int c);

#endif
