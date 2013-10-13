#include <msp430g2553.h>
#include <stdio.h>
#include <string.h>

#include "servo.h"
#include "uart.h"
#include "simplemesh.h"

static void __inline__ brief_pause(register unsigned int n)
{
    __asm__ __volatile__ (
		"1: \n"
		" dec	%[n] \n"
		" jne	1b \n"
        : [n] "+r"(n));
}

#define CMDLINE_SIZE 32

void test_servo_ctl()
{
	//	brief_pause(10000);
	servo_read_params("0 20000 1400 400");
	servo_read_params("1 20000 1500 500");
	servo_read_val("0 50");
	servo_read_val("1 -50");
	servo_print_cur(0);
	servo_print_cur(1);
}

void parse_commands()
{
	char cmdline[CMDLINE_SIZE];
	while (1) {
		int left = uart_read_line(cmdline, CMDLINE_SIZE);
		if (left >= (CMDLINE_SIZE - 3)) {
			printf("Command too short\r\n");
			continue;
		}
		switch (cmdline[0]) {
		case 'p' : 
			if (servo_read_params(cmdline+2) != 0)
				printf("Couldn't read params\r\n");
			break;
		case 's' : 
			if (servo_read_val(cmdline+2) != 0)
				printf("Couldn't read value\r\n");
			break;
		case 'g' :
			servo_print_cur(0);
			servo_print_cur(1);
			break;
		default:
			printf("Command not recognized\r\n");
		}
	}
}

int putchar(int c)
{
	return uart_send_char(c);
}

int main()
{
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

	P1DIR |= BIT0 | BIT6;
	P1OUT = BIT0 | BIT6;
	init_uart(UART_BR_115200);
	init_pwm();


	char packet_buf[SM_MAX_PACKET_SIZE];
	char data[] = "TEST";
			
	while (1) {
		P1OUT ^= BIT0;
		int packet_size = make_send_data_command(packet_buf, 14,
		                                         0x0004,
		                                         data, sizeof(data) - 1);
		uart_send_data(packet_buf, packet_size);
		/* printf("TEST\r\n"); */
		brief_pause(60000); 
	}
}
