#ifndef __MOTORS_H__
#define __MOTORS_H__

#include <stdint.h>

#define TIMER_INTERVAL_RELOAD   19890
#define DUTYCYCLE_GRANULARITY   78

void motors_init();
void motors_set_m1(uint32_t);
void motors_set_m2(uint32_t);
void motors_set_m3(uint32_t);
void motors_set_m4(uint32_t);

#endif
