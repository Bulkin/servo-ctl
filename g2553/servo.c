#include "servo.h"

#include <msp430g2553.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	uint period;
	uint mean;
	uint dev;
	int val;
} servo_ctl;

servo_ctl servos[] = { { 20000, 1500, 500, 0 },
                       { 20000, 1500, 500, 0 } };

/* init 2 channels for now */
void init_pwm()
{
	P2DIR |= BIT1 | BIT4;
	P2SEL |= BIT1 | BIT4;
	TA1CCR0 = servos[0].period;                          // PWM Period
	TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
	TA1CCR1 = servos[0].mean;                            // CCR1 PWM duty cycle
	TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
	TA1CCR2 = servos[1].mean;                            // CCR2 PWM duty cycle
	TA1CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, up mode, clear TAR	

}

void servo_set_params(uint ch, uint period, uint mean, uint dev)
{
	servos[ch].period = period;
	servos[ch].mean = mean;
	servos[ch].dev = dev;
	TA1CCR0 = servos[ch].period;
	servo_set_val(ch, servos[ch].val);
}

void servo_set_val(uint ch, int val)
{
	if ((val > 100) && (val < -100))
		return;
	servos[ch].val = val;
	int sign = val < 0 ? -1 : 1;
	val = val*sign;
	/* uses the fact that 100*dev doesn't overflow 16 bit int;
	   TODO: implement proper scaling */
	val = val * servos[ch].dev / 100;
	val = servos[ch].mean + val * sign;
	switch (ch) {
	case 0: TA1CCR1 = val; break;
	case 1: TA1CCR2 = val; break;
	}

}

char *next_token(char *str, char del)
{
	int flag = 0;
	for (int i = 0; str[i] != 0; ++i)
		if (str[i] == del)
			flag = 1;
		else 
			if (flag == 1)
				return str + i;
	return NULL;
}

int servo_read_params(char* params)
{
	char *ptr = params;
	if (NULL == ptr) return 1;
	uint ch = strtoul(ptr, NULL, 10);
	
	ptr = next_token(ptr, ' '); 
	if (NULL == ptr) return 1; 
	uint period = strtoul(ptr, NULL, 10);

	ptr = next_token(ptr, ' '); 
	if (NULL == ptr) return 1; 
	uint mean = strtoul(ptr, NULL, 10);

	ptr = next_token(ptr, ' '); 
	if (NULL == ptr) return 1; 
	uint dev = strtoul(ptr, NULL, 10);

	servo_set_params(ch, period, mean, dev);

	return 0;
}

int servo_read_val(char *val)
{
	char *ptr = val;
	if (NULL == ptr) return 1;
	uint ch = strtoul(ptr, NULL, 10);
	
	ptr = next_token(ptr, ' '); 
	if (NULL == ptr) return 1; 
	int v = strtol(ptr, NULL, 10);
	
	servo_set_val(ch, v);
	return 0;
}

void servo_print_cur(uint ch) 
{
	printf("%u %u %u %u %d\r\n", ch, servos[ch].period, servos[ch].mean,
	       servos[ch].dev, servos[ch].val);
}

