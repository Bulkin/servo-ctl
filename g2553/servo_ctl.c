#include <msp430g2553.h>
#include <stdio.h>
#include <string.h>

#include "timer.h"
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

static int servo_watchdog_counter = 0;

void servo_watchdog(unsigned int counter)
{
	static int last_value = 0;
	if (counter % 5 == 0) {
		if (servo_watchdog_counter == last_value)
			servo_read_val("0 0");
		else
			last_value = servo_watchdog_counter;
	}
}

void parse_command(char *cmdline, int size)
{
	if (size < 4) {
		printf("e Command too short\n");
		return;
	}
	switch (cmdline[0]) {
	case 'p' : 
		if (servo_read_params(cmdline+2) != 0)
			printf("e Couldn't read params: %s\n", cmdline+2);
		break;
	case 's' : 
		if (servo_read_val(cmdline+2) != 0)
			printf("e Couldn't read value: %s\n", cmdline+2);
		servo_watchdog_counter += 1;
		break;
	case 'g' :
		servo_print_cur(0);
		servo_print_cur(1);
		break;
	default:
		printf("e Command not recognized\n");
	}
}

void dispatch_cmd(uint8_t *cmd, int size)
{
	switch (cmd[0]) {
	case CMD_RX_DATA_INDICATION:
		parse_command(cmd + 6, size - 6);
		break;
	}
}

#define MAX_MSG_SIZE (SM_MAX_PACKET_SIZE - 10)
#define DEST_ADDR 0x0004

static char packet_buf[SM_MAX_PACKET_SIZE];

int putchar(int c)
{
	static uint8_t buf[MAX_MSG_SIZE];
	static int idx;
	uint8_t p_id;
	
	buf[idx++] = c;
	if ((idx == MAX_MSG_SIZE) || c == '\n') {
		int packet_size = make_send_data_command(packet_buf, p_id++,
		                                         DEST_ADDR,
		                                         buf, idx);
		uart_send_data(packet_buf, packet_size);
		idx = 0;
	}

	return c;
}

void test_loop()
{
	while (1) {
		P1OUT ^= BIT0 | BIT6;
		printf("test\n");
		brief_pause(60000); 
	}
}

void blink_led(unsigned int cnt)
{
	P1OUT ^= BIT0;
}

void blink_led_2(unsigned int cnt)
{
	P1OUT ^= BIT6;
}

int main()
{
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	P1DIR |= BIT0 | BIT6;
	P1OUT = BIT6;

	timer_add(50000, blink_led);
	timer_add(50000, servo_watchdog);
	timer_init();

	init_uart(UART_BR_115200);
	int packet_size = make_uart_9600_command(packet_buf);
	uart_send_data(packet_buf, packet_size);

	brief_pause(60000);
	init_uart(UART_BR_9600);

	init_pwm();
	servo_read_params("0 20000 1500 500");
	servo_read_params("1 20000 1500 500");
	servo_read_val("0 0");

	while (1) {
		P1OUT ^= BIT6;		
		sm_parse_packet_stream(uart_getchar());
	}
}
