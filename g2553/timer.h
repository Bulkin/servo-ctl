#define MAX_TIMER_ACTIONS 4
#define TIMER_MOD 16384

typedef void (*timer_action)(unsigned int);

void timer_init(void);
int timer_add(unsigned int interval, timer_action f);
