#include "config.h"

#ifndef __KF_H_
#define __KF_H_

typedef struct kalman_filter {
    float q_angle, q_bias, r_measure;
    float angle, bias, rate;
    float P[2][2];
} kalman_filter_t;

// reset the kalman filter
void kalman_reset(kalman_filter_t*);

// set variances to kalman filter
void kalman_set_variances(kalman_filter_t*, float q_angle, float q_bias, float r_measure);

// update filter structure with new data
void kalman_update(kalman_filter_t*, float new_angle, float new_rate, float dt);

// get estimated angle from filter
float kalman_get_angle(kalman_filter_t*);

#endif
