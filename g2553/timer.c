#include "timer.h"

#include <msp430g2553.h>

static struct {
	struct {
		timer_action func;
		unsigned int interval;
	} action_table[MAX_TIMER_ACTIONS];
	unsigned int tick;
	unsigned int counter;
} timer_state = { {0}, 50000, 0 };

static int find_free_spot()
{
	int i = 0;
	for (; i < MAX_TIMER_ACTIONS; i++)
		if (timer_state.action_table[i].interval == 0)
			break;
	return i;
}

static void
__attribute__((__interrupt__(TIMER0_A0_VECTOR)))
timer_int()
{
	timer_state.counter += 1;
	timer_state.counter %= TIMER_MOD;

	for (int i = 0; i < MAX_TIMER_ACTIONS; i++) {
		unsigned int step = (timer_state.action_table[i].interval / 
		                     timer_state.tick);
		if (timer_state.counter % step == 0)
			timer_state.action_table[i].func(timer_state.counter / step);
	}
}

void timer_init(void)
{
	CCTL0 = CCIE;                             // CCR0 interrupt enabled
	CCR0 = timer_state.tick;
	TACTL = TASSEL_2 + MC_1;                  // SMCLK, upmode
}

static unsigned int gcd(unsigned int a, unsigned int b)
{
	if (b == 0)
		return a;
	return gcd(b, a % b);
}

static unsigned int interval_gcd(unsigned int new_interval) 
{
	unsigned int res = new_interval;
	for (int i = 0; i < MAX_TIMER_ACTIONS; i++) {
		unsigned int interval = timer_state.action_table[i].interval;
		if (interval)
			res = gcd(res, timer_state.action_table[i].interval);
	}
	return res;
}

int timer_add(unsigned int interval, timer_action f)
{
	int idx = find_free_spot();
	if (idx >= MAX_TIMER_ACTIONS)
		return -1;

	timer_state.tick = interval_gcd(interval);

	timer_state.action_table[idx].func = f;
	timer_state.action_table[idx].interval = interval;
	
	return idx;
}
