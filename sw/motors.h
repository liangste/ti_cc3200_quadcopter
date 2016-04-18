#ifndef __MOTORS_H__
#define __MOTORS_H__

#include <stdint.h>

#define TIMER_INTERVAL_RELOAD   19890
#define DUTYCYCLE_GRANULARITY   78
#define MAX_THROTTLE_VALUE 254
#define MIN_THROTTLE_VALUE 0

void motors_init();

uint8_t motors_correct_throttle(int32_t);

// set throttle values for motor, [0, 255]
void motors_set_m1(uint8_t);
void motors_set_m2(uint8_t);
void motors_set_m3(uint8_t);
void motors_set_m4(uint8_t);

#endif
