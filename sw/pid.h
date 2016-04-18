#include <stdint.h>
#include "config.h"

#ifndef __PID_H_
#define __PID_H_

typedef struct pid_data {
    float k_p, k_i, k_d;
    double error_sum;
    int32_t value;
} pid_data_t;

// initialize PID and set coefficients
void pid_init(pid_data_t*, float k_p, float k_i, float k_d);

// reset pid states
void pid_reset(pid_data_t*);

// update PID structure with current desired angle, measured angle, and angular
// rate from gyro
void pid_update(pid_data_t*, double desired, double measured, double rate);

// correct throttle with this pid
int32_t pid_get_value(pid_data_t*);

#endif __PID_H__
