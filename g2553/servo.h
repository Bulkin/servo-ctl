#ifndef SERVO_H
#define SERVO_H

typedef unsigned int uint;

void init_pwm();
void servo_set_params(uint ch, uint period, uint mean, uint dev);
void servo_set_val(uint ch, int val); /* -100 to 100 */
int servo_read_params(char* params);
int servo_read_val(char *val);
void servo_print_cur(uint ch);

#endif
