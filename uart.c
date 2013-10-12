#include <msp430g2553.h>

#include "uart.h"

#define UART_BUF_LEN 32


void set_uart_9600()
{
	UCA0BR0 = 104;                            // 1MHz 9600
	UCA0BR1 = 0;                              // 1MHz 9600
	UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
}

void set_uart_115200()
{
	UCA0BR0 = 8;                              // 1MHz 115200
	UCA0BR1 = 0;                              // 1MHz 115200
	UCA0MCTL = UCBRS2 + UCBRS0;               // Modulation UCBRSx = 5
}

void init_uart(baudrate br)
{
	BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
	DCOCTL = CALDCO_1MHZ;
	P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
	P1SEL2 = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
 
	switch (br) {
	case UART_BR_9600:
		set_uart_9600();
		break;
	case UART_BR_115200:
		set_uart_115200();
		break;
	}

	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}

typedef struct {
	int start;
	int end;
	int size;
	char* buf;
} ringbuf;

static char _uartbufrx[UART_BUF_LEN];
ringbuf uartbufrx = { 0, 0, UART_BUF_LEN, _uartbufrx};

void ringbuf_add(ringbuf* rb, char c)
{
	__disable_interrupt();
	if (rb->end != (rb->start-1)) {
		rb->buf[rb->end] = c;
		rb->end = (rb->end + 1) % rb->size;
	}
	__enable_interrupt();
}

char ringbuf_get(ringbuf* rb)
{
	char c;
	__disable_interrupt();
	if (rb->end != rb->start) {
		c = rb->buf[rb->start];
		rb->start = (rb->start + 1) % rb->size;
	}
	__enable_interrupt();
	return c;
}

int ringbuf_check(ringbuf* rb)
{
	return (rb->end != rb->start);
}

static void
__attribute__((__interrupt__(USCIAB0RX_VECTOR)))
usci0rx()
{
	P1OUT ^= BIT6;
	char c = UCA0RXBUF;
	ringbuf_add(&uartbufrx, c);
	__bic_status_register_on_exit(LPM4_bits);
}

int uart_send_char(int c)
{
	while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
	UCA0TXBUF = c;            
	P1OUT ^= BIT0;
	return c;
}

char uart_getchar()
{
	while (!ringbuf_check(&uartbufrx))
		/* wait for data */
		__bis_status_register(LPM0_bits | GIE);
	return ringbuf_get(&uartbufrx);
}

int uart_read_line(char* dest, int max)
{
	while (max > 1) {
		char c = uart_getchar();
		if ((c == '\r') || c == '\n')
			break;
		*dest++ = c;
		max -= 1;
	}
	*dest = 0;
	return max;
}

int uart_send_data(char* data, int data_size)
{
	for (int i = 0; i < data_size; ++i)
		uart_send_char(data[i]);
}
